#include <boost/test/unit_test.hpp>
#include <string>

// Подключаем твои заголовочные файлы
#include "robin_hashtable.hpp"
#include "my_siphash.hpp"
#include "my_equal.hpp" // Или как называется твой функтор сравнения

BOOST_AUTO_TEST_SUITE(robin_hashtable_suite)

// Задаем алиасы для удобства тестирования
using Hash = zubarev::SipHash;
using Equal = zubarev::Equaler< std::string >;
using Table = zubarev::RobinHashTable< std::string, size_t, Hash, Equal >;

BOOST_AUTO_TEST_CASE(default_constructor)
{
  Table h;
  BOOST_CHECK(h.empty());
  BOOST_CHECK_EQUAL(h.size(), 0);
  BOOST_CHECK_EQUAL(h.capacity(), 16); // Дефолт в сигнатуре explicit RobinHashTable(size_t cap = 16)
  BOOST_CHECK(h.begin() == h.end());
}

BOOST_AUTO_TEST_CASE(constructor_with_capacity)
{
  Table h(32);
  BOOST_CHECK(h.empty());
  BOOST_CHECK_EQUAL(h.capacity(), 32);

  // Проверка защиты от нулевой емкости (твой конструктор сбрасывает в 8)
  Table h_zero(0);
  BOOST_CHECK_EQUAL(h_zero.capacity(), 8);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  Table a;
  a.add("apple", 1);
  a.add("banana", 2);

  Table copy(a);

  BOOST_CHECK_EQUAL(copy.size(), 2);
  BOOST_CHECK(copy.has("apple"));
  BOOST_CHECK(copy.has("banana"));
  BOOST_CHECK_EQUAL(copy.at("apple"), 1);
  BOOST_CHECK_EQUAL(copy.at("banana"), 2);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  Table a;
  a.add("apple", 1);
  a.add("banana", 2);

  Table moved(std::move(a));

  BOOST_CHECK_EQUAL(moved.size(), 2);
  BOOST_CHECK(moved.has("apple"));
  BOOST_CHECK_EQUAL(moved.at("apple"), 1);

  // Старый объект должен быть корректно очищен (table.slots_ = topit::Vector<Node>())
  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  Table a;
  a.add("apple", 1);

  Table copy;
  copy = a;

  BOOST_CHECK(copy.has("apple"));
  BOOST_CHECK_EQUAL(copy.at("apple"), 1);

  // Проверка самоприсваивания
  // copy = copy;
  BOOST_CHECK_EQUAL(copy.at("apple"), 1);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  Table a;
  a.add("apple", 1);

  Table moved;
  moved = std::move(a);

  BOOST_CHECK(moved.has("apple"));
  BOOST_CHECK_EQUAL(moved.at("apple"), 1);
}

BOOST_AUTO_TEST_CASE(operator_square_brackets)
{
  Table a;
  // Вставка через []
  a["apple"] = 123;
  BOOST_CHECK(a.has("apple"));
  BOOST_CHECK_EQUAL(a["apple"], 123);

  // Обновление через []
  a["apple"] = 456;
  BOOST_CHECK_EQUAL(a["apple"], 456);
}

BOOST_AUTO_TEST_CASE(at_exception_handling)
{
  Table h;
  h.add("apple", 123);

  BOOST_CHECK_EQUAL(h.at("apple"), 123);
  BOOST_CHECK_THROW(h.at("banana"), std::out_of_range);

  // Для константной таблицы
  const Table& h_const = h;
  BOOST_CHECK_EQUAL(h_const.at("apple"), 123);
  BOOST_CHECK_THROW(h_const.at("banana"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(add_and_update)
{
  Table h;
  auto it1 = h.add("apple", 10);
  BOOST_CHECK_EQUAL((*it1).val_, 10);
  BOOST_CHECK_EQUAL(h.size(), 1);

  // Дубликат ключа должен перезаписать значение
  auto it2 = h.add("apple", 20);
  BOOST_CHECK_EQUAL((*it2).val_, 20);
  BOOST_CHECK_EQUAL(h.at("apple"), 20);
  BOOST_CHECK_EQUAL(h.size(), 1); // Размер не должен увеличиться
}

BOOST_AUTO_TEST_CASE(drop_elements)
{
  Table h;
  h.add("apple", 10);
  h.add("banana", 20);

  BOOST_CHECK_EQUAL(h.drop("apple"), 10);
  BOOST_CHECK(!h.has("apple"));
  BOOST_CHECK_EQUAL(h.size(), 1);

  // Попытка удалить несуществующий элемент должна бросать out_of_range
  BOOST_CHECK_THROW(h.drop("cherry"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(rehash_and_load_factor)
{
  Table h(4); // Маленький размер для быстрого триггера рехэша
  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3); // Тут load_factor равен 3/4 = 0.75, должен сработать автоматический rehash в add()

  // BOOST_CHECK(h.capacity() > 4);
  BOOST_CHECK(h.has("a"));
  BOOST_CHECK(h.has("b"));
  BOOST_CHECK(h.has("c"));

  // Ручной рехэш
  size_t old_cap = h.capacity();
  h.rehash(old_cap * 2);
  BOOST_CHECK_EQUAL(h.capacity(), old_cap * 2);
  BOOST_CHECK_EQUAL(h.at("b"), 2);
}

BOOST_AUTO_TEST_CASE(iterators_traverse)
{
  Table h;
  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3);

  size_t count = 0;
  for (auto it = h.begin(); it != h.end(); ++it) {
    BOOST_CHECK(it->occupied_);
    count++;
  }
  BOOST_CHECK_EQUAL(count, 3);

  // Проверка константных итераторов cbegin/cend
  const Table& h_const = h;
  size_t const_count = 0;
  for (auto it = h_const.cbegin(); it != h_const.cend(); ++it) {
    BOOST_CHECK(it->occupied_);
    const_count++;
  }
  BOOST_CHECK_EQUAL(const_count, 3);
}

BOOST_AUTO_TEST_CASE(swap_tables)
{
  Table h1;
  h1.add("apple", 1);

  Table h2;
  h2.add("banana", 2);
  h2.add("cherry", 3);

  h1.swap(h2);

  BOOST_CHECK_EQUAL(h1.size(), 2);
  BOOST_CHECK(h1.has("banana"));
  BOOST_CHECK(h1.has("cherry"));

  BOOST_CHECK_EQUAL(h2.size(), 1);
  BOOST_CHECK(h2.has("apple"));
}

BOOST_AUTO_TEST_SUITE_END()
