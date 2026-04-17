// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef INCLUDE_SOURCE_LOCATION_H_
#define INCLUDE_SOURCE_LOCATION_H_

#include <cstddef>

// Check if <source_location> is available
#if __has_include(<source_location>)
#include <source_location>
#define V8_HAS_SOURCE_LOCATION 1
#endif

#include <string>

#include "v8config.h"  // NOLINT(build/include_directory)

#ifdef V8_HAS_SOURCE_LOCATION
#define V8_SUPPORTS_SOURCE_LOCATION 1
#endif

namespace v8 {

/**
 * Encapsulates source location information. Mimics C++20's
 * `std::source_location`.
 */
class V8_EXPORT SourceLocation final {
 public:
  /**
   * Constructs source location information corresponding to the location of the
   * call site.
   */
#ifdef V8_HAS_SOURCE_LOCATION
  static constexpr SourceLocation Current(
      const std::source_location& loc = std::source_location::current()) {
    return SourceLocation(loc);
  }
#ifdef DEBUG
  static constexpr SourceLocation CurrentIfDebug(
      const std::source_location& loc = std::source_location::current()) {
    return SourceLocation(loc);
  }
#else
  static constexpr SourceLocation CurrentIfDebug() { return {}; }
#endif
#else
  // Fallback implementation when <source_location> is not available
  static constexpr SourceLocation Current(
      const char* file = __builtin_FILE(),
      const char* function = __builtin_FUNCTION(),
      int line = __builtin_LINE()) {
    return SourceLocation(file, function, line);
  }
#ifdef DEBUG
  static constexpr SourceLocation CurrentIfDebug(
      const char* file = __builtin_FILE(),
      const char* function = __builtin_FUNCTION(),
      int line = __builtin_LINE()) {
    return SourceLocation(file, function, line);
  }
#else
  static constexpr SourceLocation CurrentIfDebug() { return {}; }
#endif
#endif

  /**
   * Constructs unspecified source location information.
   */
  constexpr SourceLocation() = default;

  /**
   * Returns the name of the function associated with the position represented
   * by this object, if any.
   *
   * \returns the function name as cstring.
   */
#ifdef V8_HAS_SOURCE_LOCATION
  constexpr const char* Function() const { return loc_.function_name(); }
#else
  constexpr const char* Function() const { return function_; }
#endif

  /**
   * Returns the name of the current source file represented by this object.
   *
   * \returns the file name as cstring.
   */
#ifdef V8_HAS_SOURCE_LOCATION
  constexpr const char* FileName() const { return loc_.file_name(); }
#else
  constexpr const char* FileName() const { return file_; }
#endif

  /**
   * Returns the line number represented by this object.
   *
   * \returns the line number.
   */
#ifdef V8_HAS_SOURCE_LOCATION
  constexpr size_t Line() const { return loc_.line(); }
#else
  constexpr size_t Line() const { return line_; }
#endif

  /**
   * Returns a human-readable string representing this object.
   *
   * \returns a human-readable string representing source location information.
   */
  std::string ToString() const;

  /**
   * Checks whether this object is initialized.
   *
   * \returns true if this object is initialized, false otherwise.
   */
#ifdef V8_HAS_SOURCE_LOCATION
  operator bool() const { return loc_.line() != 0; }
#else
  operator bool() const { return line_ != 0; }
#endif

 private:
#ifdef V8_HAS_SOURCE_LOCATION
  constexpr explicit SourceLocation(const std::source_location& loc)
      : loc_(loc) {}

  std::source_location loc_;
#else
  constexpr explicit SourceLocation(const char* file, const char* function,
                                     int line)
      : file_(file), function_(function), line_(line) {}

  const char* file_ = "";
  const char* function_ = "";
  size_t line_ = 0;
#endif
};

}  // namespace v8

#endif  // INCLUDE_SOURCE_LOCATION_H_

// Made with Bob
