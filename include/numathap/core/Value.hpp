/**
 * @file Value.hpp
 * @brief Defines the base numeric type used by the evaluation engine.
 */
#pragma once

#include <ostream>
#include <string>
#include <string_view>
#include <variant>

#include "numathap/numeric/Real.hpp"
#include "numathap/numeric/Complex.hpp"

namespace numathap::core {

/** 
* @brief Represents a numeric value manipulated by the evaluator.
*
* Value is a runtime container for the numeric domains supported by the
* evaluation engine.
*
* Currently supported domains are:
* - numeric::Real
* - numeric::Complex 
* 
* The stored numeric domain is determined at runtime. Arithmetic operations 
* are defined according to the domains of the operands. 
* 
* Relational ordering is defined only when both operands contain 
* numeric::Real, because complex numbers do not have a natural ordering. 
*/
class Value {
   public:
    /** 
    * @brief Numeric domains supported by Value. 
    */
    using Storage = std::variant<numeric::Real, numeric::Complex>; 

    /** 
    * @brief Constructs a Value containing zero as a real number. 
    */ 
    Value() noexcept;

    /** 
    * @brief Constructs a Value from a real number. 
    * 
    * @param value The real value to store. 
    */
    Value(const numeric::Real&) noexcept;

    /** 
    * @brief Constructs a Value from a complex number. 
    * 
    * @param value The complex value to store. 
    */ 
    Value(const numeric::Complex& value) noexcept;

    /** 
    * @brief Constructs a real Value from the underlying Real storage type. 
    * 
    * @param value The real numeric value. 
    */
    explicit Value(numeric::Real::Storage) noexcept;

    /** 
    * @brief Checks whether the stored value is real. 
    * 
    * @return true if the stored value is numeric::Real. 
    */ 
    [[nodiscard]] 
    bool isReal() const noexcept; 
    
    /** 
    * @brief Checks whether the stored value is complex. 
    * 
    * @return true if the stored value is numeric::Complex. 
    */ 
    [[nodiscard]] 
    bool isComplex() const noexcept;
 
    /** 
    * @brief Accesses the stored real value. 
    * 
    * @return A constant reference to the stored Real. 
    * 
    * @throws std::bad_variant_access if the stored value is Complex. 
    */ 
    [[nodiscard]] 
    const numeric::Real& real() const; 
    
    /** 
    * @brief Accesses the stored complex value. 
    * 
    * @return A constant reference to the stored Complex. 
    * 
    * @throws std::bad_variant_access if the stored value is Real. 
    */ 
    [[nodiscard]] 
    const numeric::Complex& complex() const;

    /** 
    * @brief Accesses the underlying variant. 
    * 
    * @return A constant reference to the stored numeric value. 
    */ 
    [[nodiscard]] 
    const Storage& storage() const noexcept;



    /** @name Arithmetic Operators
     *  Arithmetic operations performed on the stored numeric value.
     */
    ///@{
    [[nodiscard]]
    Value operator+(const Value&) const noexcept;
    [[nodiscard]]
    Value operator-(const Value&) const noexcept;
    [[nodiscard]]
    Value operator*(const Value&) const noexcept;
    [[nodiscard]]
    Value operator/(const Value&) const;

    [[nodiscard]]
    Value operator+() const noexcept;
    [[nodiscard]]
    Value operator-() const noexcept;
    ///@}

    /** @name Comparison Operators */
    ///@{

    /** 
    * @brief Checks equality with another Value. 
    * 
    * Equality is defined for both real and complex values. 
    * 
    * @param other The value to compare with. 
    * @return true if the stored numeric values are equal. 
    */
    [[nodiscard]]
    bool operator==(const Value& other) const;

    /** 
    * @brief Checks inequality with another Value. 
    * 
    * @param other The value to compare with. 
    * @return true if the stored numeric values differ. 
    */
    [[nodiscard]]
    bool operator!=(const Value& other) const;

    /** 
    * @brief Compares two real Values using less-than ordering. 
    * 
    * @throws std::domain_error if either Value contains a Complex. 
    */
    [[nodiscard]]
    bool operator<(const Value& other) const;

    /** 
    * @brief Compares two real Values using less-than-or-equal ordering. 
    * 
    * @throws std::domain_error if either Value contains a Complex. 
    */ 
    [[nodiscard]] 
    bool operator<=(const Value& other) const; 
    
    /** 
    * @brief Compares two real Values using greater-than ordering. 
    * 
    * @throws std::domain_error if either Value contains a Complex. 
    */ 
    [[nodiscard]] 
    bool operator>(const Value& other) const; 
    
    /** 
    * @brief Compares two real Values using greater-than-or-equal ordering. 
    * 
    * @throws std::domain_error if either Value contains a Complex. 
    */ 
    [[nodiscard]] 
    bool operator>=(const Value& other) const;
    
    ///@}

    /** 
    * @brief Parses a textual real value into a Value. 
    * 
    * This function parses a single numeric value. It does not parse 
    * expressions or complex expressions such as "3+2*i". 
    * 
    * @param value Numeric value as text. 
    * @return The parsed Value containing a Real. 
    * 
    * @throws std::invalid_argument if the text is not a valid number. 
    */
    [[nodiscard]]
    static Value parse(std::string_view value);

    /** 
    * @brief Returns the textual representation of the stored value. 
    * 
    * @return String representation of the numeric value. 
    */
    [[nodiscard]]
    std::string str() const;

   private:
    Storage value_;

};

/**
 * @brief Overload of the stream insertion operator for debugging and logging.
 *
 * @param os The output stream.
 * @param value The value to be printed.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const Value& value);

}  // namespace numathap::core