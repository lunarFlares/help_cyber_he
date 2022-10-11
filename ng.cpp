#include <array>
#include <cstddef>
/**
 * @brief const string.
 */
class ConstStr {
  size_t size;
  const char* data;

 public:
  /**
   * @brief Construct from const char* and size.
   *
   */
  constexpr ConstStr(const char* _data, size_t _size)
      : data(_data), size(_size) {}
  /**
   * @brief Construct from const char* (ends with '\0'.)
   *
   */
  constexpr ConstStr(const char* _data) : data(_data), size(0) {
    while (*(_data++)) size++;
  }
  /**
   * @brief Default constructor.
   *
   */
  constexpr ConstStr() : data(nullptr), size(0) {}
  /**
   * @brief Substring from specified position `pos`. This operation will not
   * copy the string.
   *
   * @param pos Start position.
   * @return constexpr ConstStr
   */
  constexpr ConstStr substr(size_t pos) const {
    if (pos >= size) return ConstStr(nullptr, 0);
    return ConstStr(data + pos, size - pos);
  }
  /**
   * @brief Substring `len` chars from specified position `pos`. This operation
   * will not copy the string.
   *
   * @param pos Start position.
   * @param len Count.
   * @return constexpr ConstStr
   */
  constexpr ConstStr substr(size_t pos, size_t len) const {
    if (pos >= size) return ConstStr(nullptr, 0);
    if (pos + len >= size) return substr(pos);
    return ConstStr(data + pos, len);
  }
  /**
   * @brief Returns the size of the string.
   *
   * @return constexpr size_t
   */
  constexpr size_t length() const { return size; }
  /**
   * @brief Returns the raw const char* of the string.
   *
   * @return constexpr const char*
   */
  constexpr const char* c_str() const { return data; }
  /**
   * @brief Returns the char on specified position `pos`.
   *
   * @param pos Position.
   * @return constexpr char
   */
  constexpr char operator[](size_t pos) const { return data[pos]; }
  /**e
   * @brief Compare the differences between two strings.
   *
   * @param val
   * @return constexpr int
   */
  constexpr int compare(const ConstStr& val) const {
    if (val.size > size)
      return 1;
    else if (val.size < size)
      return -1;
    for (size_t i = 0; i < size; i++) {
      if (data[i] > val[i])
        return 1;
      else if (data[i] < val[i])
        return -1;
    }
    return 0;
  }
  /**
   * @brief Returns true if string is equal to the string `val`.
   *
   * @param val The string to be compared.
   * @return true if the string is equal to `val`.
   * @return false if the string is not equal to `val`.
   */
  constexpr bool operator==(const ConstStr& val) const {
    return compare(val) == 0;
  }
  /**
   * @brief Returns true if string is not equal to the string `val`.
   *
   * @param val The string to be compared.
   * @return true if the string is not equal to `val`.
   * @return false if the string is equal to `val`.
   */
  constexpr bool operator!=(const ConstStr& val) const {
    return !operator==(val);
  }
  /**
   * @brief Returns true if string is greater than the string `val`.
   *
   * @param val The string to be compared.
   * @return true if the string is greater than `val`.
   * @return false if the string is not greater than `val`.
   */
  constexpr bool operator>(const ConstStr& val) const {
    return compare(val) > 0;
  }
  /**
   * @brief Returns true if string is less than the string `val`.
   *
   * @param val The string to be compared.
   * @return true if the string is less than `val`.
   * @return false if the string is not less than `val`.
   */
  constexpr bool operator<(const ConstStr& val) const {
    return compare(val) < 0;
  }
};
/**
 * @brief Split the string `str` by delim `delim`.
 *
 * @tparam size Expected size.
 * @param str The string to be splitted.
 * @param delim Delim character.
 * @return constexpr std::array<ConstStr, size>
 */
template <size_t size>
constexpr std::array<ConstStr, size> split(const ConstStr& str, char delim) {
  std::array<ConstStr, size> ret;
  size_t pos = 0, len = 0, cur = 0;
  for (size_t i = 0; i < str.length(); i++) {
    if (str[i] == delim) {
      ret[cur++] = str.substr(pos, len);
      if (cur == size) return ret;
      pos = i + 1;
      len = 0;
    } else
      len++;
  }
  ret[size - 1] = str.substr(pos, len);
  return ret;
}
/**
 * @brief Count the number of copies of a character in a string.
 *
 * @param str The string.
 * @param chr The value to be counted.
 * @return constexpr size_t
 */
constexpr size_t count(const ConstStr& str, char chr) {
  size_t sum = 0;
  for (size_t i = 0; i < str.length(); i++) {
    if (str[i] == chr) sum++;
  }
  return sum;
}
/**
 * @brief Checks if the character `digit` is a valid digit.
 *
 * @param digit The character.
 * @return true if the character `digit` is a valid digit.
 * @return false if the character `digit` is not a valid digit.
 */
constexpr bool isdigit_constexpr(char digit) {
  return digit >= '0' && digit <= '9';
}
/**
 * @brief Convert string `str` to specified type `T`.
 *
 * @tparam T The specified type. Default to size_t.
 * @param str The string.
 * @return constexpr T
 */
template <typename T = size_t>
constexpr T stoi(const ConstStr& str) {
  T ret = 0;
  for (size_t i = 0; i < str.length(); i++) {
    if (!isdigit_constexpr(str[i])) continue;
    ret *= 10;
    ret += str[i] - '0';
  }
  return ret;
}
template <size_t size>
constexpr std::array<size_t, size> process_u8(const ConstStr& str) {
  std::array<ConstStr, size> arr = split<size>(str, '|');
  std::array<size_t, size> ret;
  for (size_t i = 0; i < arr.size(); i++) {
    ret[i] = stoi(arr[i]);
  }
  return ret;
}
#define U8(str) process_u8<count(#str, '|') + 1>(ConstStr(#str))

template <size_t size = 1>
struct u8 {
  std::array<size_t, size> arr;
  constexpr u8() : arr({}) {}
  constexpr u8(std::array<size_t, size> s) : arr(s) {}
  constexpr u8(size_t data) {
    static_assert(size != 0, "Assertion failed: size != 0");
    arr[0] = data;
  }
  constexpr operator size_t() {
    static_assert(size != 0, "Assertion failed: size != 0");
    return arr[0];
  }
  constexpr operator bool() {
    static_assert(size != 0, "Assertion failed: size != 0");
    return arr[0];
  }
  constexpr bool operator<(int n) const { return arr[0] < n; }
  constexpr void operator++(int) { (*this) = (*this) - (-1); }
  constexpr size_t& operator[](size_t idx) { return arr[idx]; }
  constexpr const size_t& operator[](size_t idx) const { return arr[idx]; }
  constexpr u8<size> operator-(size_t n) const {
    u8<size> ret = *this;
    for (size_t i = 0; i < size; i++) {
      ret[i] -= n;
    }
    return ret;
  }
  constexpr u8<size> operator-(int n) const {
    u8<size> ret = *this;
    for (size_t i = 0; i < size; i++) {
      ret[i] -= n;
    }
    return ret;
  }
};
template <size_t size>
struct U8Arr {
  std::array<size_t, size> arr;
  u8<size> index_list;
  constexpr U8Arr() : arr({}) {}
  template <size_t index_size>
  constexpr U8Arr& operator[](const u8<index_size>& index) {
    static_assert(size >= index_size, "Assertion failed: size >= index_size");
    for (size_t i = 0; i < index_size; i++) {
      index_list[i] = index[i];
    }
    for (size_t i = index_size; i < size; i++) {
      index_list[i] = index[index_size - 1];
    }
    return *this;
  }
  constexpr U8Arr<size>& operator=(size_t data) {
    for (size_t i = 0; i < size; i++) {
      arr[index_list[i]] = data;
    }
    return *this;
  }
};
U8Arr<68> forceCon;
template <size_t size, size_t size_2>
constexpr void powerCon(u8<size> whichKey, u8<size_2> force) {
  if (whichKey) {
    forceCon[whichKey - 1] = force;
  } else {
    for (u8 i = 0; i < 68; i++) {
      forceCon[i] = force;
    }
  }
}

#define powerCon(nums, force) \
  powerCon<count(#nums, '|') + 1, 1>(u8(U8(#nums)), force)
constexpr void littleFingerForce() {
  powerCon(0, 100);
  powerCon(1 | 2 | 6 | 7 | 11 | 52 | 57 | 58 | 65, 10);
}

#include <iostream>

int main() {
  littleFingerForce();
  for (size_t i = 0; i < forceCon.arr.size(); i++) {
    int el = forceCon.arr[i];
    std::cout << i << ": " << el << std::endl;
  }
}
