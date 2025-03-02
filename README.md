# yajson

![status sheild](https://img.shields.io/static/v1?label=test+coverage&message=99%&color=active&style=plastic)
![GitHub](https://img.shields.io/github/license/marcpage/yajson?style=plastic)
[![commit sheild](https://img.shields.io/github/last-commit/marcpage/yajson?style=plastic)](https://github.com/marcpage/yajson/commits)
[![activity sheild](https://img.shields.io/github/commit-activity/m/marcpage/yajson?style=plastic)](https://github.com/marcpage/yajson/commits)
![GitHub top language](https://img.shields.io/github/languages/top/marcpage/yajson?style=plastic)
[![size sheild](https://img.shields.io/github/languages/code-size/marcpage/yajson?style=plastic)](https://github.com/marcpage/yajson)
[![issues sheild](https://img.shields.io/github/issues-raw/marcpage/yajson?style=plastic)](https://github.com/marcpage/yajson/issues)
[![follow sheild](https://img.shields.io/github/followers/marcpage?label=Follow&style=social)](https://github.com/marcpage?tab=followers)
[![watch sheild](https://img.shields.io/github/watchers/marcpage/yajson?label=Watch&style=social)](https://github.com/marcpage/yajson/watchers)

Yet Another JSON: C++ light-weight JSON parser and formatter

## Features

- Single header file, less than 1,500 lines of code
- Supports C++11 and later
- Pure C++: supports Linux, macOS, and Windows
- Supports full [RFC 8259](https://datatracker.ietf.org/doc/html/rfc8259) compliance
  - With the exception of adopting ECMAScript 6's `\u{XXXXXX}` unicode support
  - Only generated when Unicode is outside of the supported range for RFC 8259
  - Always allowed in parsing
- Uses dictionary and array semantics as well as methods
- Can create via parsing text or programatically
- Compact or human-readable text formatting available
- Over 90% unit test code coverage

## Example Usage

```C++
#include "yajson/yajson.h"
#include <iostream>

int main() {
    auto json = R"(
        {
            "test\"me\"": [
                1,
                2.0,
                {"go\/now":3},
                {"eol": "\r\n"},
                {"ht": "\t"},
                {"vt": "\f", "bell": "\b"},
                true,
                false,
                null,
                "C:\\"
            ]
        }
        )";

    auto object = yajson::Value::parse(json);

    object["key"] = yajson::Value::object()
                        .set("real", 3.14)
                        .set("integer", 42)
                        .set("null", yajson::Value::null())
                        .set("list", yajson::Value::array()
                            .append("JSON Rocks")
                            .append(true));

    std::cout << object.format() << std::endl;
    std::cout << std::endl << object.format(2);
    return 0;
}
```

Which produces:

```json
{"key":{"integer":42,"list":["JSON Rocks",true],"null":null,"real":3.140000},"test\"me\"":[1,2.000000,{"go\/now":3},{"eol":"\r\n"},{"ht":"\t"},{"bell":"\b","vt":"\f"},true,false,null,"C:\\"]}

{
  "key":{
    "integer":42,
    "list":[
      "JSON Rocks",
      true
    ],
    "null":null,
    "real":3.140000
  },
  "test\"me\"":[
    1,
    2.000000,
    {
      "go\/now":3
    },
    {
      "eol":"\r\n"
    },
    {
      "ht":"\t"
    },
    {
      "bell":"\b",
      "vt":"\f"
    },
    true,
    false,
    null,
    "C:\\"
  ]
}
```