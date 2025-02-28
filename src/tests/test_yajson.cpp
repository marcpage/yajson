#include "../yajson/yajson.h"
#include <stdio.h>

static auto json = R"(
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

static bool testParse() {
    bool success = true;
    auto j1 = yajson::Value::parse(json);
    auto js1 = j1.format();
    auto j2 = yajson::Value::parse(js1);
    auto js2 = j2.format();
    auto js3 = j2.format(4);
    auto j3 = yajson::Value::parse(js3);

    success = success && js1 == js2;
    success = success && js1 != js3;
    success = success && j1 == j2;
    success = success && j1 == j3;

    if (!success) {
        printf("FAIL %s\n", __func__);
    }
    
    return success;
}

static bool testIs() {
    bool success = true;
    auto j1 = yajson::Value::parse(json);

    success = success && j1.is(yajson::Value::Object);
    success = success && j1["test\"me\""].is(yajson::Value::Array);
    success = success && j1["test\"me\""][0].is(yajson::Value::Integer);
    success = success && j1["test\"me\""][6].is(yajson::Value::Boolean);
    success = success && j1["test\"me\""][7].is(yajson::Value::Boolean);
    success = success && j1["test\"me\""][8].is(yajson::Value::Null);
    success = success && j1["test\"me\""][9].is(yajson::Value::String);

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testArray() {
    bool success = true;
    auto j1 = yajson::Value::parse(json);

    success = success && j1.is(yajson::Value::Object);
    success = success && j1["test\"me\""].is(yajson::Value::Array);
    success = success && j1["test\"me\""].count() == 10;

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testObject() {
    bool success = true;
    auto j1 = yajson::Value::parse(json);

    success = success && j1.is(yajson::Value::Object);
    success = success && j1.count() == 1;
    success = success && j1.keys().size() == 1;
    success = success && j1.keys()[0] == "test\"me\"";
    success = success && j1["test\"me\""][2].is(yajson::Value::Object);
    success = success && j1["test\"me\""][2].count() == 1;
    success = success && j1["test\"me\""][2].keys()[0] == "go/now";
    success = success && j1["test\"me\""][2]["go/now"].integer() == 3;
    success = success && j1["test\"me\""][3].is(yajson::Value::Object);
    success = success && j1["test\"me\""][3].count() == 1;
    success = success && j1["test\"me\""][3].keys()[0] == "eol";
    success = success && j1["test\"me\""][3]["eol"].string() == "\r\n";
    success = success && j1["test\"me\""][4].is(yajson::Value::Object);
    success = success && j1["test\"me\""][4].count() == 1;
    success = success && j1["test\"me\""][4].keys()[0] == "ht";
    success = success && j1["test\"me\""][4]["ht"].string() == "\t";
    success = success && j1["test\"me\""][5].is(yajson::Value::Object);
    success = success && j1["test\"me\""][5].count() == 2;
    success = success && j1["test\"me\""][5]["vt"].string() == "\f";
    success = success && j1["test\"me\""][5]["bell"].string() == "\b";

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testCreate() {
    bool success = true;
    auto j1 = yajson::Value::object();

    j1["array"] = yajson::Value::array();
    j1["object"] = yajson::Value::object();
    j1["string"] = "test string";
    j1["integer"] = 5;
    j1["real"] = 3.14;
    j1["null"] = yajson::Value::null();
    j1["array"][0] = yajson::Value::object();
    j1["array"].insert(yajson::Value::array());
    j1["array"][2] = std::string("another test string");
    j1["array"][3] = 7;
    j1["array"][4] = 2.7;
    j1["array"].append(yajson::Value());
    j1["object"]["array"] = yajson::Value::array();
    j1["object"]["object"] = yajson::Value::object();
    j1["object"]["string"] = "yet another test string";
    j1["object"]["integer"] = 42;
    j1["object"]["real"] = 0.675;
    j1["object"]["true"] = true;
    j1["object"]["false"] = false;
    j1["object"].set("null", yajson::Value::null());

    auto js1 = j1.format();
    auto j2 = yajson::Value::parse(js1);
    auto js2 = j2.format();
    auto js3 = j2.format(50);
    const auto j3 = yajson::Value::parse(js3);

    success = success && j1 == j2;
    success = success && j1 == j3;
    success = success && js1 == js2;

    success = success && j3.has("array");
    success = success && j3.has("object");
    success = success && j3.has("string");
    success = success && j3.has("integer");
    success = success && j3.has("real");
    success = success && j3.has("null");
    success = success && !j3.has("void");

    success = success && j3.is(yajson::Value::Object);
    success = success && j3["array"].is(yajson::Value::Array);
    success = success && j3["object"].is(yajson::Value::Object);
    success = success && j3["string"].string() == "test string";
    success = success && j3["string"] == yajson::Value("test string");
    success = success && j3["integer"].integer() == 5;
    success = success && j3["integer"] == yajson::Value(5);
    success = success && j3["real"] == yajson::Value(3.14);
    success = success && j3["null"] == yajson::Value();
    success = success && j3["null"] == yajson::Value::null();

    success = success && j3["array"][0].is(yajson::Value::Array);
    success = success && j3["array"][1].is(yajson::Value::Object);
    success = success && j3["array"][2].string() == "another test string";
    success = success && j3["array"][2] == yajson::Value(std::string("another test string"));
    success = success && j3["array"][3].integer() == 7;
    success = success && j3["array"][3] == yajson::Value(int64_t(7));
    success = success && j3["array"][4] == yajson::Value(2.7);
    success = success && j3["array"][5] == yajson::Value();
    success = success && j3["array"][6] == yajson::Value::null();

    success = success && j3["object"].has("true");
    success = success && j3["object"].has("false");
    success = success && j3["object"]["array"].is(yajson::Value::Array);
    success = success && j3["object"]["object"].is(yajson::Value::Object);
    success = success && j3["object"]["string"].string() == "yet another test string";
    success = success && j3["object"]["string"] == yajson::Value("yet another test string");
    success = success && j3["object"]["integer"].integer() == 42;
    success = success && j3["object"]["integer"] == yajson::Value(42);
    success = success && j3["object"]["real"] == yajson::Value(0.675);
    success = success && j3["object"]["null"] == yajson::Value();
    success = success && j3["object"]["null"] == yajson::Value::null();
    success = success && j3["object"]["true"].boolean();
    success = success && !j3["object"]["false"].boolean();

    success = success && j3["object"] != yajson::Value::null();
    success = success && j3["array"] != yajson::Value::null();
    success = success && j3["string"] != yajson::Value::null();
    success = success && j3["integer"] != yajson::Value::null();
    success = success && j3["real"] != yajson::Value::null();
    success = success && j3["object"]["true"] != yajson::Value::null();
    success = success && j3["null"] != yajson::Value::array();

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testNull() {
    bool success = true;
    auto j1 = yajson::Value();
    const auto j2 = yajson::Value();

    j1.clear();

    success = success && j1.is(yajson::Value::Null);
    success = success && j1.isNull();
    success = success && !j1.boolean();
    success = success && j1.integer() == 0;
    success = success && j1.real() <= 0 && j1.real() >= 0;
    success = success && j1.string().size() == 0;
    success = success && j1.count() == 0;
    success = success && j1.keys().size() == 0;
    success = success && !j1.has("anything");
    success = success && j1.format() == "null";

    try {
        j1.set("anything", "value");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.get("anything");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j2.get("anything");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.append("value");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.insert("value");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.get(0);
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j2.get(0);
        success = false;
    } catch(const std::domain_error&) {}

    try {
        const auto value = j1["anything"];
        success = false;
    } catch(const std::domain_error&) {}

    try {
        const auto value = j2["anything"];
        success = false;
    } catch(const std::domain_error&) {}

    try {
        const auto value = j1[0];
        success = false;
    } catch(const std::domain_error&) {}

    try {
        const auto value = j2[0];
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.erase(0);
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.erase("anything");
        success = false;
    } catch(const std::domain_error&) {}

    success = success && j1 == j2;

    j1 = 5;
    success = success && j1.integer() == 5;

    j1 = yajson::Value::null();
    success = success && j1.isNull();

    j1 += 1;
    success = success && j1.integer() == 1;

    j1 = yajson::Value::null();
    success = success && j1.isNull();

    j1 -= 5;
    success = success && j1.integer() == -5;

    j1 = yajson::Value::null();
    success = success && j1.isNull();

    j1 += int64_t(7);
    success = success && j1.integer() == 7;

    j1 = yajson::Value::null();
    success = success && j1.isNull();

    j1 -= int64_t(42);
    success = success && j1.integer() == -42;

    j1 = yajson::Value::null();
    success = success && j1.isNull();

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testDelete() {
    bool success = true;
    auto j1 = yajson::Value::parse(json);

    j1["test\"me\""].erase(6,8); // remove true and false
    j1["test\"me\""].erase(7); // remove C:\ from the end
    j1["test\"me\""][4].clear(); // clear out ht
    j1["test\"me\""][5].erase("bell");

    success = success && j1["test\"me\""].count() == 7;
    success = success && j1["test\"me\""][4].is(yajson::Value::Object);
    success = success && j1["test\"me\""][4].count() == 0;
    success = success && j1["test\"me\""][5].count() == 1;
    success = success && j1["test\"me\""][6].isNull();

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testWrongType() {
    bool success = true;
    auto j1 = yajson::Value::parse(json);

    try {
        j1.boolean();
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.integer();
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.real();
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.string();
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""].erase("key");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.erase(0);
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""][0].count();
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""].keys();
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""].has("test");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""].get("test");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1.get(0);
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""][0].clear();
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""][0].erase(1);
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""][0].erase("test");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""][0].set("test", "value");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""][0].append("value");
        success = false;
    } catch(const std::domain_error&) {}

    try {
        j1["test\"me\""][0].insert("value");
        success = false;
    } catch(const std::domain_error&) {}

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testInvalidJson() {
    bool success = true;

    try {
        yajson::Value::parse("[trust]");
        success = false;
    } catch(const std::invalid_argument&) {}

    try {
        yajson::Value::parse("[9223372036854775808]");
        success = false;
    } catch(const std::invalid_argument&) {}

    try {
        yajson::Value::parse("[...]");
        success = false;
    } catch(const std::invalid_argument&) {}

    try {
        yajson::Value::parse("[\"\\u{110000}\"]");
        success = false;
    } catch(const std::invalid_argument&) {}

    try {
        yajson::Value::parse("[\"\\i\"]");
        success = false;
    } catch(const std::invalid_argument&) {}

    try {
        const auto j = yajson::Value::parse("[\"\x80xyz\"]");
        j.format();
        success = false;
    } catch(const std::invalid_argument&) {}    

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testUnicode() {
    bool success = true;
    const auto j = yajson::Value::parse(R"(
    [
        "skull = \u2620",
        "A = \u0041",
        "umbrella = \u2602",
        "heart eyes = \u{1F60D}",
        "copyright = \u00A9"
    ]
    )");
    const auto jText = j.format();
    const auto j2 = yajson::Value::parse(jText);

    success = success && j2.count() == 5;
    success = success && j2 == j;

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testMiscellaneous() {
    bool success = true;
    auto j = yajson::Value::array()
                .append(3.14159265) // 1
                .append(42) // 2
                .append(true) // 3
                .insert(yajson::Value::object() // 0
                        .set("string", "value"));
    
    success = success && j[0] != yajson::Value::array();
    success = success && j[0]["string"] != yajson::Value::object();
    success = success && j[1] != yajson::Value::object();
    success = success && j[2] != yajson::Value::object();
    success = success && j[3] != yajson::Value::object();
    success = success && j != yajson::Value::object();
    success = success && j[1].real() < 3.14159266;
    success = success && j[1].real() > 3.14159264;
    success = success && j[1] == 3.14159265;
    success = success && j[1] != 3.14159264;
    success = success && j[1] != 3.14159266;

    success = success && j.count() == 4;

    j.erase(5);

    success = success && j.count() == 4;

    j.clear();

    success = success && j.count() == 0;
    
    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

static bool testArrayInsertConditions() {
    auto success = true;

    yajson::Value array = yajson::Value::array();
    array.append(yajson::Value(1));
    array.append(yajson::Value(2));
    success = success && (array.count() == 2);

    array.insert(yajson::Value(3), 1);
    success = success && (array.count() == 3);
    success = success && (array[0].integer() == 1);
    success = success && (array[1].integer() == 3);
    success = success && (array[2].integer() == 2);

    array.insert(yajson::Value(4), 3);
    success = success && (array.count() == 4);
    success = success && (array[0].integer() == 1);
    success = success && (array[1].integer() == 3);
    success = success && (array[2].integer() == 2);
    success = success && (array[3].integer() == 4);

    array.insert(yajson::Value(5), 10);
    success = success && (array.count() == 5);
    success = success && (array[4].integer() == 5);

    if (!success) {
        printf("FAIL %s\n", __func__);
    }

    return success;
}

int main(const int /*argc*/, const char* const /*argv*/[]) {
    int failures = 0;

    failures += testParse() ? 0 : 1;
    failures += testIs() ? 0 : 1;
    failures += testObject() ? 0 : 1;
    failures += testArray() ? 0 : 1;
    failures += testCreate() ? 0 : 1;
    failures += testNull() ? 0 : 1;
    failures += testDelete() ? 0 : 1;
    failures += testWrongType() ? 0 : 1;
    failures += testInvalidJson() ? 0 : 1;
    failures += testUnicode() ? 0 : 1;
    failures += testMiscellaneous() ? 0 : 1;
    failures += testArrayInsertConditions() ? 0 : 1;

    if (failures > 0) {
        printf("FAIL %d tests\n", failures);
    }
    
    return failures;
}
