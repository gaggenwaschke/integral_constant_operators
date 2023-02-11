#include <catch2/catch.hpp>
#include <concepts>
#include <std_extensions/integral_constant_operators>
#include <tuple>

namespace mock {
using number_types = std::tuple<std::uint8_t,
                                std::uint16_t,
                                std::uint32_t,
                                std::uint64_t,
                                std::int8_t,
                                std::int16_t,
                                std::int32_t,
                                std::int64_t>;

template<typename...>
struct cat;

template<>
struct cat<>
{
  using type = std::tuple<>;
};

template<typename... _Ts>
struct cat<std::tuple<_Ts...>>
{
  using type = std::tuple<_Ts...>;
};

template<typename... _T1s, typename... _T2s, typename... _Rem>
struct cat<std::tuple<_T1s...>, std::tuple<_T2s...>, _Rem...>
{
  using type = typename cat<std::tuple<_T1s..., _T2s...>, _Rem...>::type;
};

template<typename left_type, typename... right_types>
struct apply_each
{
  using type = std::tuple<std::pair<left_type, right_types>...>;
};

template<typename, typename>
struct cross_product;

template<typename... left_types, typename... right_types>
struct cross_product<std::tuple<left_types...>, std::tuple<right_types...>>
{
  using type = typename cat<typename apply_each<left_types, right_types...>::type...>::type;
};

using cross_product_numbers = cross_product<mock::number_types, mock::number_types>::type;
} // namespace mock

namespace test {
template<typename>
struct is_integral_constant : std::false_type
{};

template<typename value_type, value_type value>
struct is_integral_constant<std::integral_constant<value_type, value>> : std::true_type
{};
} // namespace test

TEST_CASE("integral constant disjunction", "[extension][operator]")
{
  CHECK(std::same_as<std::true_type, decltype(std::true_type{} || std::true_type{})>);
  CHECK(std::same_as<std::true_type, decltype(std::true_type{} || std::false_type{})>);
  CHECK(std::same_as<std::true_type, decltype(std::false_type{} || std::true_type{})>);
  CHECK(std::same_as<std::false_type, decltype(std::false_type{} || std::false_type{})>);

  CHECK(std::same_as<bool, decltype(std::declval<bool>() || std::declval<bool>())>);

  CHECK(std::same_as<std::true_type, decltype(std::true_type{} || std::declval<bool>())>);
  CHECK(std::same_as<std::true_type, decltype(std::declval<bool>() || std::true_type{})>);

  CHECK(std::same_as<bool, decltype(std::false_type{} || std::declval<bool>())>);
  CHECK(std::same_as<bool, decltype(std::declval<bool>() || std::false_type{})>);
}

TEST_CASE("integral constant conjunction", "[extension][operator]")
{
  CHECK(std::same_as<std::true_type, decltype(std::true_type{} && std::true_type{})>);
  CHECK(std::same_as<std::false_type, decltype(std::true_type{} && std::false_type{})>);
  CHECK(std::same_as<std::false_type, decltype(std::false_type{} && std::true_type{})>);
  CHECK(std::same_as<std::false_type, decltype(std::false_type{} && std::false_type{})>);

  CHECK(std::same_as<bool, decltype(std::declval<bool>() && std::declval<bool>())>);

  CHECK(std::same_as<std::false_type, decltype(std::false_type{} && std::declval<bool>())>);
  CHECK(std::same_as<std::false_type, decltype(std::declval<bool>() && std::false_type{})>);

  CHECK(std::same_as<bool, decltype(std::true_type{} && std::declval<bool>())>);
  CHECK(std::same_as<bool, decltype(std::declval<bool>() && std::true_type{})>);
}

TEMPLATE_LIST_TEST_CASE("integral constant addition", "[extension][operator]", mock::cross_product_numbers)
{
  using left_type = typename TestType::first_type;
  using right_type = typename TestType::second_type;

  std::integral_constant<left_type, left_type{ 2 }> two{};
  std::integral_constant<right_type, right_type{ 4 }> four{};

  auto result = two + four;

  CHECK(result == 6);
  CHECK(test::is_integral_constant<decltype(result)>::value);
}

TEMPLATE_LIST_TEST_CASE("integral constant subtraction", "[extension][operator]", mock::cross_product_numbers)
{
  using left_type = typename TestType::first_type;
  using right_type = typename TestType::second_type;

  std::integral_constant<left_type, left_type{ 4 }> four{};
  std::integral_constant<right_type, right_type{ 2 }> two{};

  auto result = four - two;

  CHECK(result == 2);
  CHECK(test::is_integral_constant<decltype(result)>::value);
}

TEMPLATE_LIST_TEST_CASE("integral constant multiplication", "[extension][operator]", mock::cross_product_numbers)
{
  using left_type = typename TestType::first_type;
  using right_type = typename TestType::second_type;

  std::integral_constant<left_type, left_type{ 2 }> two{};
  std::integral_constant<right_type, right_type{ 4 }> four{};

  auto result = two * four;

  CHECK(result == 8);
  CHECK(test::is_integral_constant<decltype(result)>::value);
}

TEMPLATE_LIST_TEST_CASE("integral constant division", "[extension][operator]", mock::cross_product_numbers)
{
  using left_type = typename TestType::first_type;
  using right_type = typename TestType::second_type;

  std::integral_constant<left_type, left_type{ 2 }> two{};
  std::integral_constant<right_type, right_type{ 4 }> four{};

  auto result = four / two;

  CHECK(result == 2);
  CHECK(test::is_integral_constant<decltype(result)>::value);
}
