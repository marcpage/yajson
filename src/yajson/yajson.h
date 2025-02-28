#pragma once

#include <string>
#include <vector>
#include <limits>
#include <memory>
#include <sstream>
#include <iomanip>
#include <map>
#include <iterator>
#include <algorithm>

namespace yajson {

class Instance;

class Value {
public:
    typedef std::vector<std::string> StringList;
    enum Type {Object, Array, Integer, Boolean, Real, String, Null};

    static Value parse(const std::string& jsonText, size_t* offset=nullptr);
    static Value null();
    static Value array();
    static Value object();

    Value();
    Value(const std::string& value);
    Value(int value);
    Value(int64_t value);
    Value(double value);
    Value(const char* value);
    Value(bool value);
    
    Value(const Value& other);
    ~Value()=default;

    Type getType() const;
    bool is(Type valueType) const;
    bool boolean() const;
    int64_t integer() const;
    double real() const;
    std::string string() const;
    bool isNull() const;
    
    int count() const;
    StringList keys() const;
    bool has(const std::string& key) const;
    Value& get(const std::string& key);
    const Value& get(const std::string& key) const;
    Value& get(size_t index);
    const Value& get(size_t index) const;

    std::string &format(std::string &buffer, int indent=-1, int indentLevel=0) const;
    std::string format(int indent=-1, int indentLevel=0) const;

    Value& clear();
    Value& erase(size_t start, size_t end=std::numeric_limits<size_t>::max());
    Value& erase(const std::string& key);

    Value& set(const std::string& key, const Value& value);
    Value& append(const Value& value);
    Value& insert(const Value& value, size_t before=0);

    Value& operator=(const Value& other);
    Value& operator=(int value);
    Value& operator=(int64_t value);
    Value& operator=(double value);
    Value& operator=(const std::string &value);
    Value& operator=(const char *value);
    Value& operator=(bool value);
    Value& operator+=(int64_t value);
    Value& operator+=(int value);
    Value& operator-=(int64_t value);
    Value& operator-=(int value);
    Value& operator[](size_t index);
    const Value& operator[](size_t index) const;
    Value& operator[](const std::string &key);
    const Value& operator[](const std::string &key) const;
    bool operator==(const Value &other) const;
    bool operator!=(const Value &other) const;

    typedef std::unique_ptr<Instance> InstPtr;
    static size_t skipWhitespace(const std::string& text, size_t offset);
private:
    Value(InstPtr instance);
    InstPtr _instance;

    static void _parseWord(const std::string& text, const std::string& word, size_t& offset);
    static Value _parseNumber(const std::string& text, size_t& offset);
};

class Instance {
public:
    Instance()=default;
    virtual ~Instance()=default;

    virtual Instance *clone() const=0;
    virtual void format(std::string &buffer, int indent, int indentLevel) const=0;
    virtual bool equals(const Instance& other) const;
    virtual Value::Type getType() const=0;

    virtual bool boolean() const;
    virtual int64_t integer() const;
    virtual double real() const;
    virtual std::string string() const;
    virtual int count() const;
    virtual Value::StringList keys() const;
    virtual bool has(const std::string& key);
    virtual Value& get(const std::string& key);
    virtual Value& get(size_t index);
    virtual void clear();
    virtual void erase(size_t start, size_t end);
    virtual void erase(const std::string& key);
    virtual void set(const std::string& key, const Value& value);
    virtual void append(const Value& value);
    virtual void insert(const Value& value, size_t before);

    Instance(const Instance&)=delete;
    Instance& operator=(const Instance&)=delete;
};

class String : public Instance {
public:
    static std::string parse(const std::string& text, size_t& offset);

    String(const std::string& value);
    virtual ~String()=default;

    virtual Instance *clone() const override;
    virtual void format(std::string &buffer, int indent, int indentLevel) const override;
    void _formatCodepoint(std::string& buffer, size_t &i) const;
    virtual bool equals(const Instance& other) const override;
    virtual Value::Type getType() const override;

    virtual std::string string() const override;

    String(const String&)=delete;
    String& operator=(const String&)=delete;
private:
    std::string _value;
    static size_t _codepoint(const std::string &text, size_t &offset);
    static std::string _utf8(size_t codepoint);   
    static void _parseEscaped(const std::string& text, size_t& offset, std::string& result);
    static void _parseEscapedUnicode(const std::string& text, size_t& offset, std::string& result);
};

class Integer : public Instance {
public:
    Integer(const int64_t value);
    virtual ~Integer()=default;

    virtual Instance *clone() const override;
    virtual void format(std::string &buffer, int indent, int indentLevel) const override;
    virtual bool equals(const Instance& other) const override;
    virtual Value::Type getType() const override;

    virtual int64_t integer() const override;

    Integer(const Integer&)=delete;
    Integer& operator=(const Integer&)=delete;
private:
    int64_t _value;
};
    
class Real : public Instance {
public:
    Real(const double value);
    virtual ~Real()=default;

    virtual Instance *clone() const override;
    virtual void format(std::string &buffer, int indent, int indentLevel) const override;
    virtual bool equals(const Instance& other) const override;
    virtual Value::Type getType() const override;

    virtual double real() const override;

    Real(const Real&)=delete;
    Real& operator=(const Real&)=delete;
private:
    double _value;
};

class Boolean : public Instance {
public:
    Boolean(const bool value);
    virtual ~Boolean()=default;

    virtual Instance *clone() const override;
    virtual void format(std::string &buffer, int indent, int indentLevel) const override;
    virtual bool equals(const Instance& other) const override;
    virtual Value::Type getType() const override;

    virtual bool boolean() const override;

    Boolean(const Boolean&)=delete;
    Boolean& operator=(const Boolean&)=delete;
private:
    bool _value;
};

class Array : public Instance {
public:
    static Array* parse(const std::string& text, size_t& offset);

    Array(const std::vector<Value>& value);
    virtual ~Array()=default;

    virtual Instance *clone() const override;
    virtual void format(std::string &buffer, int indent, int indentLevel) const override;
    virtual bool equals(const Instance& other) const override;
    virtual Value::Type getType() const override;

    virtual int count() const override;
    virtual Value& get(const std::string& key) override;
    virtual Value& get(size_t index) override;
    virtual void clear() override;
    virtual void erase(const std::string& key) override;
    virtual void erase(size_t start, size_t end) override;
    virtual void append(const Value& value) override;
    virtual void insert(const Value& value, size_t before) override;

    Array(const Array&)=delete;
    Array& operator=(const Array&)=delete;
private:
    std::vector<Value> _value;
};

class Object : public Instance {
public:
    static Object* parse(const std::string& text, size_t& offset);
    
    Object(const std::map<std::string, Value>& value);
    virtual ~Object()=default;

    virtual Instance *clone() const override;
    virtual void format(std::string &buffer, int indent, int indentLevel) const override;
    virtual bool equals(const Instance& other) const override;
    virtual Value::Type getType() const override;

    virtual int count() const override;
    virtual Value::StringList keys() const override;
    virtual bool has(const std::string& key) override;
    virtual Value& get(const std::string& key) override;
    virtual Value& get(size_t index) override;
    virtual void clear() override;
    virtual void erase(const std::string& key) override;
    virtual void erase(size_t start, size_t end) override;
    virtual void set(const std::string& key, const Value& value) override;

    Object(const Object&)=delete;
    Object& operator=(const Object&)=delete;
private:
    std::map<std::string, Value> _value;
};

#define YaJsonAssert(expression) if (!(expression)) {throw std::invalid_argument("Failed: " #expression);} else

inline Value Value::parse(const std::string& jsonText, size_t* position) {
    Value value;
    size_t offset = nullptr == position ? 0 : *position;

    offset = skipWhitespace(jsonText, offset);
    YaJsonAssert(offset < jsonText.size());
    
    switch (jsonText[offset]) {
        case '{': 
            value._instance = InstPtr(Object::parse(jsonText, offset));
            break;
        case '[':
            value._instance = InstPtr(Array::parse(jsonText, offset));
            break;
        case '"':
            value = String::parse(jsonText, offset);
            break;
        case 't':
            _parseWord(jsonText, "true", offset);
            value = true;
            break;
        case 'f':
            _parseWord(jsonText, "false", offset);
            value = false;
            break;
        case 'n':
            _parseWord(jsonText, "null", offset);
            break;
        default:
            value = _parseNumber(jsonText, offset);
            break;
    }

    if (nullptr != position) {
        *position = offset;
    }

    return value;
}

inline Value Value::null() {
    return Value();
}

inline Value Value::array() {
    return Value(std::unique_ptr<Instance>(new yajson::Array({})));
}

inline Value Value::object() {
    return Value(std::unique_ptr<Instance>(new yajson::Object({})));
}

inline Value::Value()
    :_instance() {}
    
inline Value::Value(const std::string& value)
    :_instance() {
    *this = value;
}

inline Value::Value(int value)
    :_instance() {
    *this = value;
}

inline Value::Value(int64_t value)
    :_instance() {
    *this = value;
}

inline Value::Value(double value)
    :_instance() {
    *this = value;
}

inline Value::Value(const char* value)
    :_instance() {
    *this = value;
}

inline Value::Value(bool value)
    :_instance() {
    *this = value;
}

inline Value::Value(const Value& other)
    :_instance() {
    if (!other.isNull()) {
        _instance = InstPtr(other._instance->clone());
    }
}

inline bool Value::boolean() const {
    return isNull() ? false : _instance->boolean();
}

inline Value::Type Value::getType() const {
    return isNull() ? Null : _instance->getType();
}

inline bool Value::is(Type valueType) const {
    return getType() == valueType;
}

inline int64_t Value::integer() const {
    return isNull() ? 0 : _instance->integer();
}

inline double Value::real() const {
    return isNull() ? 0.0 : _instance->real();
}

inline std::string Value::string() const {
    return isNull() ? std::string() : _instance->string();
}

bool Value::isNull() const {
    return nullptr == _instance;
}

inline int Value::count() const {
    return isNull() ? 0 : _instance->count();
}

inline Value::StringList Value::keys() const {
    return isNull() ? StringList() : _instance->keys();
}

inline bool Value::has(const std::string& key) const {
    return isNull() ? false : _instance->has(key);
}

inline Value& Value::get(const std::string& key) {
    if (isNull()) {
        throw std::domain_error("null");
    }

    return _instance->get(key);
}

inline const Value& Value::get(const std::string& key) const {
    if (isNull()) {
        throw std::domain_error("null");
    }

    return _instance->get(key);
}

inline Value& Value::get(size_t index) {
    if (isNull()) {
        throw std::domain_error("null");
    }

    return _instance->get(index);
}

inline const Value& Value::get(size_t index) const {
    if (isNull()) {
        throw std::domain_error("null");
    }

    return _instance->get(index);
}

inline std::string &Value::format(std::string &buffer, int indent, int indentLevel) const {
    if (isNull()) {
        return buffer = "null";
    }

    _instance->format(buffer, indent, indentLevel);
    return buffer;
}

inline std::string Value::format(int indent, int indentLevel) const {
    std::string buffer;

    return format(buffer, indent, indentLevel);
}

inline Value& Value::clear() {
    if (!isNull()) {
        _instance->clear();
    }

    return *this;
}

inline Value& Value::erase(size_t start, size_t end) {
    if (isNull()) {
        throw std::domain_error("null");
    }

    _instance->erase(start, end);
    return *this;
}

inline Value& Value::erase(const std::string& key) {
    if (isNull()) {
        throw std::domain_error("null");
    }

    _instance->erase(key);
    return *this;
}

inline Value& Value::set(const std::string& key, const Value& value) {
    if (isNull()) {
        throw std::domain_error("null");
    }

    _instance->set(key, value);
    return *this;
}

inline Value& Value::append(const Value& value) {
    if (isNull()) {
        throw std::domain_error("null");
    }

    _instance->append(value);
    return *this;    
}

inline Value& Value::insert(const Value& value, size_t before) {
    if (isNull()) {
        throw std::domain_error("null");
    }

    _instance->insert(value, before);
    return *this;    
}

inline Value& Value::operator=(const Value& other) {
    if (!other.isNull()) {
        _instance = InstPtr(other._instance->clone());
    } else {
        _instance = nullptr;
    }

    return *this;
}

inline Value& Value::operator=(int64_t value) {
    _instance = InstPtr(new yajson::Integer(value));
    return *this;
}

inline Value& Value::operator=(int value) {
    _instance = InstPtr(new yajson::Integer(value));
    return *this;
}

inline Value& Value::operator=(double value) {
    _instance = InstPtr(new yajson::Real(value));
    return *this;
}

inline Value& Value::operator=(const std::string &value) {
    _instance = InstPtr(new yajson::String(value));
    return *this;
}

inline Value& Value::operator=(const char *value) {
    _instance = InstPtr(new yajson::String(value));
    return *this;
}

inline Value& Value::operator=(bool value) {
    _instance = InstPtr(new yajson::Boolean(value));
    return *this;
}

inline Value& Value::operator+=(int64_t value) {
    return *this = integer() + value;
}

inline Value& Value::operator+=(int value) {
    return *this = integer() + value;
}

inline Value& Value::operator-=(int64_t value) {
    return *this = integer() - value;
}

inline Value& Value::operator-=(int value) { 
    return *this = integer() - value;
}

inline Value& Value::operator[](size_t index) {
    return get(index);
}

inline const Value& Value::operator[](size_t index) const {
    return get(index);
}

inline Value& Value::operator[](const std::string &key) {
    return get(key);
}

inline const Value& Value::operator[](const std::string &key) const {
    return get(key);
}

inline bool Value::operator==(const Value &other) const {
    if (isNull() && other.isNull()) {
        return true;
    }

    if (isNull() != other.isNull()) {
        return false;
    }

    return _instance->equals(*other._instance) && other._instance->equals(*_instance);
}

inline bool Value::operator!=(const Value &other) const {
    return !(*this == other);
}

inline Value::Value(InstPtr instance)
    :_instance(std::move(instance)) {}

inline size_t Value::skipWhitespace(const std::string& text, size_t offset) {
    while ((offset < text.length()) && ::isspace(text[offset])) {
        offset += 1;
    }

    return offset;
}

inline void Value::_parseWord(const std::string& text, const std::string& word, size_t& offset) {
    const auto actual = text.substr(offset, word.length());
    
    if (actual != word) {
        throw std::invalid_argument("Invalid word: " + actual);
    }

    offset += word.length();
}

inline Value Value::_parseNumber(const std::string& text, size_t& offset) {
    std::string integerChars("-+0123456789");
    std::string realChars("Ee.");
    bool hasRealChar = false;
    const std::string::size_type start = offset;
    size_t after = 0;
    Value value;

    while ( (offset < text.length()) 
            && ( (integerChars.find(text[offset]) != std::string::npos) 
                || (realChars.find(text[offset]) != std::string::npos) ) ) {
        hasRealChar = hasRealChar 
                        || (realChars.find(text[offset]) != std::string::npos);
        offset += 1;
    }

    YaJsonAssert(start < offset);

    if (hasRealChar) {
        try {
            value = std::stod(text.substr(start, offset - start), &after);
        } catch (const std::exception &exception) {
            throw std::invalid_argument(std::string("Illegal Number: ")  // NOTEST
                                    + text.substr(start, offset - start));
        }
    } else {
        try {
            const auto converted = std::stoll(text.substr(start, offset - start), &after);

            value = static_cast<int64_t>(converted);
        } catch (const std::exception &exception) {
            throw std::invalid_argument(std::string("Illegal Number: ") // NOTEST
                                        + text.substr(start, offset - start));
        }
    }

    YaJsonAssert(after == offset - start);
    return value;
}

inline bool Instance::boolean() const {
    throw std::domain_error("wrong type");
}

inline bool Instance::equals(const Instance& other) const {
    return getType() == other.getType();
}

inline int64_t Instance::integer() const {
    throw std::domain_error("wrong type");
}

inline double Instance::real() const {
    throw std::domain_error("wrong type");
}

inline std::string Instance::string() const {
    throw std::domain_error("wrong type");
}

inline int Instance::count() const {
    throw std::domain_error("wrong type");
}

inline Value::StringList Instance::keys() const {
    throw std::domain_error("wrong type");
}

inline bool Instance::has(const std::string& /*key*/) {
    throw std::domain_error("wrong type");
}

inline Value& Instance::get(const std::string& /*key*/) {
    throw std::domain_error("wrong type");
}

inline Value& Instance::get(size_t /*index*/) {
    throw std::domain_error("wrong type");
}

inline void Instance::clear() {
    throw std::domain_error("wrong type");
}

inline void Instance::erase(size_t /*start*/, size_t /*end*/) {
    throw std::domain_error("wrong type");
}

inline void Instance::erase(const std::string& /*key*/) {
    throw std::domain_error("wrong type");
}

inline void Instance::set(const std::string& /*key*/, const Value& /*value*/) {
    throw std::domain_error("wrong type");
}

inline void Instance::append(const Value& /*value*/) {
    throw std::domain_error("wrong type");
}

inline void Instance::insert(const Value& /*value*/, size_t /*before*/) {
    throw std::domain_error("wrong type");
}

inline std::string String::parse(const std::string& text, size_t& offset) {
    std::string result;

    offset += 1; // skip open quote

    do {
        YaJsonAssert(offset < text.length());

        switch (text[offset]) {
            case '\\':
                _parseEscaped(text, offset, result);
                break;

            case '"':
                break;

            default:
                result += text[offset];
                offset += 1;
                break;
        }

        YaJsonAssert(offset < text.length());
    } while ('"' != text[offset]);

    YaJsonAssert(offset < text.length());
    YaJsonAssert('"' == text[offset]);
    offset += 1; // skip end quote
    return result;
}

inline String::String(const std::string& value)
    :_value(value) {}

inline Instance *String::clone() const {
    return new String(_value);
}

inline void String::format(std::string &buffer, int /*indent*/, int /*indentLevel*/) const {
    buffer = "\"";

    for (std::string::size_type i = 0; i < _value.length(); ++i) {
        switch (_value[i]) {
            case '\\':
                buffer += "\\\\";
                break;
            case '"':
                buffer += "\\\"";
                break;
            case '/':
                buffer += "\\/";
                break;
            case '\t':
                buffer += "\\t";
                break;
            case '\r':
                buffer += "\\r";
                break;
            case '\n':
                buffer += "\\n";
                break;
            case '\b':
                buffer += "\\b";
                break;
            case '\f':
                buffer += "\\f";
                break;
            default:
                _formatCodepoint(buffer, i);
                break;
        }
    }

    buffer += "\"";
}

inline void String::_formatCodepoint(std::string &buffer, size_t &i) const
{
    auto offset = i;
    const auto codepoint = _codepoint(_value, offset);

    if (offset - i == 1) { // single character, just copy it across
        buffer += _value[i];
    } else {
        const bool ecma6 = (codepoint > 0xFFFF); // u{xxxxxx} not supported before ecma6
        std::stringstream stream;

        if (ecma6) { // just copy the value across, since escaping may not be supported
            buffer = buffer + _value.substr(i, offset - i);
        } else {
            buffer = buffer + "\\u";
            stream << std::setfill('0') << std::setw(4) << std::hex << codepoint;
            buffer = buffer + stream.str();
        }
    }

    i = offset - 1;
}

inline bool String::equals(const Instance& other) const {
    if (!Instance::equals(other)) {
        return false;
    }

    return _value == dynamic_cast<const String*>(&other)->_value;
}

inline Value::Type String::getType() const {
    return Value::String;
}

inline std::string String::string() const {
    return _value;
}

inline size_t String::_codepoint(const std::string &text, std::string::size_type &offset) {
    /*
            1  7   U+0000   U+007F   0xxxxxxx
            2  11  U+0080   U+07FF   110xxxxx  10xxxxxx
            3  16  U+0800   U+FFFF   1110xxxx  10xxxxxx  10xxxxxx
            4  21  U+10000  U+10FFFF 11110xxx  10xxxxxx  10xxxxxx  10xxxxxx
    */
    const bool oneByte = (offset < text.length()) && ((0x80 & text.data()[offset]) == 0);
    const bool twoBytes = (offset + 1 < text.length()) && ((0xE0 & text.data()[offset]) == 0xC0);
    const bool threeBytes = (offset + 2 < text.length()) && ((0xF0 & text.data()[offset]) == 0xE0);
    const bool fourBytes = (offset + 3 < text.length()) && ((0xF8 & text.data()[offset]) == 0xF0);
    size_t codepoint;

    if (oneByte) {
        offset += 1;
        codepoint = text.data()[offset - 1] & 0xFF;
        YaJsonAssert(codepoint <= 0x7F);
    } else if (twoBytes) { // NOTEST
        YaJsonAssert(offset + 2 <= text.length());
        offset += 2;
        codepoint = ((size_t(text.data()[offset - 2] & 0x1F) << 6)
                    | size_t(text.data()[offset - 1] & 0x3F));
        YaJsonAssert((text.data()[offset - 1] & 0xC0) == 0x80);
        YaJsonAssert((codepoint > 0x7F) && (codepoint <= 0x7FF));
    } else if (threeBytes) {
        YaJsonAssert(offset + 3 <= text.length());
        offset += 3;
        codepoint = ((size_t(text.data()[offset - 3] & 0x0F) << 12)
                  |  (size_t(text.data()[offset - 2] & 0x3F) << 6)
                  |   size_t(text.data()[offset - 1] & 0x3F));
        YaJsonAssert((text.data()[offset - 1] & 0xC0) == 0x80);
        YaJsonAssert((text.data()[offset - 2] & 0xC0) == 0x80);
        YaJsonAssert((codepoint > 0x7FF) && (codepoint <= 0xFFFF));
    } else if (fourBytes) {
        YaJsonAssert(offset + 4 <= text.length());
        offset += 4;
        codepoint = ((size_t(text.data()[offset - 4] & 0x07) << 18)
                   | (size_t(text.data()[offset - 3] & 0x3F) << 12)
                   | (size_t(text.data()[offset - 2] & 0x3F) << 6)
                   |  size_t(text.data()[offset - 1] & 0x3F));
        YaJsonAssert((text.data()[offset - 1] & 0xC0) == 0x80);
        YaJsonAssert((text.data()[offset - 2] & 0xC0) == 0x80);
        YaJsonAssert((text.data()[offset - 3] & 0xC0) == 0x80);
        YaJsonAssert((codepoint > 0xFFFF) && (codepoint <= 0x10FFFF));
    } else { // NOTEST
        throw std::invalid_argument("invalid codepoint: " + text.substr(offset, 4));
    }

    return codepoint;
}

inline std::string String::_utf8(size_t codepoint) {
    /*
        1  7   U+0000   U+007F    0xxxxxxx
        2  11  U+0080   U+07FF    110xxxxx  10xxxxxx
        3  16  U+0800   U+FFFF    1110xxxx  10xxxxxx  10xxxxxx
        4  21  U+10000  U+10FFFF  11110xxx  10xxxxxx  10xxxxxx  10xxxxxx
    */
    std::string value;

    if (codepoint <= 0x7F) {
        value.assign(1, char(codepoint));
    } else if (codepoint <= 0x7FF) {
        char buffer[3]; // NOTEST

        buffer[0] = static_cast<char>((6 << 5) | (codepoint >> 6));
        buffer[1] = static_cast<char>((2 << 6) | (codepoint & 0x3F));
        buffer[2] = 0;
        value = buffer;
    } else if (codepoint <= 0xFFFF) {
        char buffer[4];

        buffer[0] = static_cast<char>((14 << 4) | ( codepoint >> 12));
        buffer[1] = static_cast<char>((2  << 6) | ((codepoint >> 6) & 0x3F));
        buffer[2] = static_cast<char>((2  << 6) | ( codepoint & 0x3F));
        buffer[3] = 0;
        value = buffer;
    } else if (codepoint <= 0x10FFFF) {
        char buffer[5];

        buffer[0] = static_cast<char>((30 << 3) | (codepoint >> 18));
        buffer[1] = static_cast<char>((2 << 6) | ((codepoint >> 12) & 0x3F));
        buffer[2] = static_cast<char>((2 << 6) | ((codepoint >> 6) & 0x3F));
        buffer[3] = static_cast<char>((2 << 6) | (codepoint & 0x3F));
        buffer[4] = 0;
        value = buffer;
    } else {
        throw std::invalid_argument("invalid codepoint: " 
                                    + std::to_string(codepoint));
    }

    return value;
}

inline void String::_parseEscaped(const std::string& text, size_t& offset, std::string& result) {
    offset += 1; // skip backslash (\) escape character

    switch (text[offset]) {
        case '/':
            result += '/';
            break;
        case '"':
            result += '"';
            break;
        case '\\':
            result += '\\';
            break;
        case 'b':
            result += '\b';
            break;
        case 'f':
            result += '\f';
            break;
        case 'r':
            result += '\r';
            break;
        case 'n':
            result += '\n';
            break;
        case 't':
            result += '\t';
            break;
        case 'u':
            _parseEscapedUnicode(text, offset, result);
            break;
        default:
            throw std::invalid_argument(std::string("Illegal escape: ") + text[offset]);
    }
    
    offset += 1;
}

inline void String::_parseEscapedUnicode(const std::string& text, size_t& offset, std::string& result) {
    long value;
    size_t count = 0;

    if (text[offset + 1] == '{') {
        const std::string::size_type end = text.find('}', offset + 2);
        const std::string::size_type size = end - offset - 2;

        YaJsonAssert(std::string::npos != end);
        YaJsonAssert(offset + 2 != end);
        value = std::stol(text.substr(offset + 2, size), &count, 16);
        YaJsonAssert(count == size);
        offset += 1 + size + 1;
    } else {
        value = std::stol(text.substr(offset + 1, 4), &count, 16);
        YaJsonAssert(count == 4);
        offset += 4;
    }

    result += _utf8(static_cast<size_t>(value));
}

inline Integer::Integer(const int64_t value)
    :_value(value) {}

inline Instance *Integer::clone() const {
    return new Integer(_value);
}

inline void Integer::format(std::string &buffer, int /*indent*/, int /*indentLevel*/) const {
    buffer = std::to_string(_value);
}

inline bool Integer::equals(const Instance& other) const {
    if (!Instance::equals(other)) {
        return false;
    }

    return _value == dynamic_cast<const Integer*>(&other)->_value;
}

inline Value::Type Integer::getType() const {
    return Value::Integer;
}

inline int64_t Integer::integer() const {
    return _value;
}

inline Real::Real(const double value)
    :_value(value) {}

inline Instance *Real::clone() const {
    return new Real(_value);
}

inline void Real::format(std::string &buffer, int /*indent*/, int /*indentLevel*/) const {
    buffer = std::to_string(_value);
}

inline bool Real::equals(const Instance& other) const {
    if (!Instance::equals(other)) {
        return false;
    }
    
    const auto a = _value;
    const auto b = dynamic_cast<const Real*>(&other)->_value;
    const auto epsilon = std::numeric_limits<double>::epsilon();
    const auto highest = std::max(std::abs(a), std::abs(b));
    return std::abs(a - b) < epsilon * highest;
}

inline Value::Type Real::getType() const {
    return Value::Real;
}

inline double Real::real() const {
    return _value;
}

inline Boolean::Boolean(const bool value)
    :_value(value) {}

inline Instance *Boolean::clone() const {
    return new Boolean(_value);
}

inline void Boolean::format(std::string &buffer, int /*indent*/, int /*indentLevel*/) const {
    buffer = _value ? "true" : "false";
}

inline bool Boolean::equals(const Instance& other) const {
    if (!Instance::equals(other)) {
        return false;
    }

    return _value == dynamic_cast<const Boolean*>(&other)->_value;
}

inline Value::Type Boolean::getType() const {
    return Value::Boolean;
}

inline bool Boolean::boolean() const {
    return _value;
}

inline Array* Array::parse(const std::string& text, size_t& offset) {
    auto result = std::unique_ptr<Array>(new Array({}));
    offset += 1; // skip [

    while ((offset < text.length()) && (text[offset] != ']')) {
        offset = Value::skipWhitespace(text, offset);

        if (']' == text[offset]) {
            break; // end of array
        }
        
        auto value = Value::parse(text, &offset);
        result->append(value);

        offset = Value::skipWhitespace(text, offset);
        YaJsonAssert(',' == text[offset] || ']' == text[offset]);

        if (',' == text[offset]) {
            offset += 1; // skip ,
            offset = Value::skipWhitespace(text, offset);
        }
    }

    offset += 1; // skip ]
    YaJsonAssert(offset <= text.length());
    return result.release();
}

inline Array::Array(const std::vector<Value>& value)
    :_value(value) {}

inline Instance *Array::clone() const {
    return new Array(_value);
}

inline void Array::format(std::string &buffer, int indent, int indentLevel) const {
    std::string value;
    std::string suffix = ",";
    std::string linePrefix = "";
    std::string lastLinePrefix = "";
    std::string lineSuffix = (indent >= 0) ? "\n" : "";

    if (indent >= 0) {
        const auto lastIndents = static_cast<size_t>(indent * indentLevel);
        const auto indents = lastIndents + static_cast<size_t>(indent);
        linePrefix.assign(indents, ' ');
        lastLinePrefix.assign(lastIndents, ' ');
    }

    buffer = "[" + lineSuffix;

    for (auto i = _value.begin(); i != _value.end(); ++i) {
        i->format(value, indent, indentLevel + 1);

        if ((i + 1) == _value.end()) {
            suffix = "";
        }

        buffer += linePrefix + value + suffix + lineSuffix;
    }

    buffer += lastLinePrefix + "]";
}

inline bool Array::equals(const Instance& other) const {
    if (!Instance::equals(other)) {
        return false;
    }

    return _value == dynamic_cast<const Array*>(&other)->_value;
}

inline Value::Type Array::getType() const {
    return Value::Array;
}

inline int Array::count() const {
    return _value.size();
}

inline Value& Array::get(const std::string& key) {
    return Instance::get(key);
}

inline Value& Array::get(size_t index) {
    while (index >= _value.size()) {
        _value.push_back(Value());
    }

    return _value[index];
}

inline void Array::clear() { // NOTEST
    _value.clear();
}

inline void Array::erase(size_t start, size_t end) {
    const auto arraySize = _value.size();

    if (start >= arraySize) {
        return;
    }

    const auto startIter = _value.begin() + static_cast<int>(start);
    const auto endIter = end >= arraySize 
                        ? _value.end() 
                        : _value.begin() + static_cast<int>(end);

    _value.erase(startIter, endIter);
}

inline void Array::erase(const std::string& key) {
    return Instance::erase(key);
}

inline void Array::append(const Value& value) {
    _value.push_back(value);
}

inline void Array::insert(const Value& value, size_t before) {
    const auto arraySize = _value.size();
    const auto beforeIter = before >= arraySize 
                            ? _value.end()  // NOTEST
                            : _value.begin() + static_cast<int>(before);

    _value.insert(beforeIter, value);
}

inline Object* Object::parse(const std::string& text, size_t& offset) {
    auto result = std::unique_ptr<Object>(new Object({}));
    offset += 1; // skip {

    while ((offset < text.length()) && (text[offset] != '}')) {
        offset = Value::skipWhitespace(text, offset);
        YaJsonAssert(offset < text.length());

        if ('}' == text[offset]) {
            break; // end of object
        }

        YaJsonAssert('"' == text[offset]);
        auto key = String::parse(text, offset);
        offset = Value::skipWhitespace(text, offset);

        YaJsonAssert(offset < text.length());
        YaJsonAssert(':' == text[offset]);
        offset += 1; // skip :

        auto value = Value::parse(text, &offset);
        result->set(key, value);
        offset = Value::skipWhitespace(text, offset);
        YaJsonAssert(',' == text[offset] || '}' == text[offset]);

        if (',' == text[offset]) {
            offset += 1; // skip ,
            offset = Value::skipWhitespace(text, offset);
        }
    }

    offset += 1; // skip }
    YaJsonAssert(offset <= text.length());
    return result.release();
}

inline Object::Object(const std::map<std::string, Value>& value)
    :_value(value) {}

inline Instance *Object::clone() const {
    return new Object(_value);
}

inline void Object::format(std::string &buffer, int indent, int indentLevel) const {
    Value key;
    std::string keyStr;
    std::string value;
    std::string linePrefix = "";
    std::string lastLinePrefix = "";
    std::string lineSuffix = (indent >= 0) ? "\n" : "";
    size_t itemsLeft = _value.size();

    if (indent >= 0) {
        const auto lastIndents = static_cast<size_t>(indent * indentLevel);
        const auto indents = lastIndents + static_cast<size_t>(indent);
        linePrefix.assign(indents, ' ');
        lastLinePrefix.assign(lastIndents, ' ');
    }

    buffer = "{" + lineSuffix;

    for (auto i : _value) {
        itemsLeft -= 1;
        key = i.first;
        key.format(keyStr, indent, indentLevel + 1);
        i.second.format(value, indent, indentLevel + 1);
        buffer += linePrefix + keyStr + ":" + value + (itemsLeft == 0 ? "" : ",")
                    + lineSuffix;
    }

    buffer += lastLinePrefix + "}";
}

inline bool Object::equals(const Instance& other) const {
    if (!Instance::equals(other)) {
        return false;
    }

    return _value == dynamic_cast<const Object*>(&other)->_value;
}

inline Value::Type Object::getType() const {
    return Value::Object;
}

inline int Object::count() const {
    return _value.size();
}

inline Value::StringList Object::keys() const {
    Value::StringList keys;

    keys.reserve(_value.size());
    std::transform(_value.begin(), _value.end(), std::back_inserter(keys),
                   [](const auto& pair) { return pair.first; });
    return keys;
}

inline bool Object::has(const std::string& key) {
    return _value.find(key) != _value.end();
}

inline Value& Object::get(const std::string& key) {
    return _value[key];
}

inline Value& Object::get(size_t index) {
    return Instance::get(index);
}

inline void Object::clear() {
    _value.clear();
}

inline void Object::erase(const std::string& key) {
    _value.erase(key);
}

inline void Object::erase(size_t start, size_t end) {
    return Instance::erase(start, end);
}

inline void Object::set(const std::string& key, const Value& value) {
    _value[key] = value;
}

#undef YaJsonAssert
}