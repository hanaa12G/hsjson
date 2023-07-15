#include <string>
#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <cmath>
#include <any>
#include <map>
#include <sstream>
#include "hsjson.hh"

using namespace hs::json;

void test_hsjson_parser()
{
  auto normal_case = []()
  {
    std::string str{R"({"name" : "John Doe", "age": 14})"};
    auto object = parse(str).get_as<json_object>();

    assert(object.get_attribute("name").get_as<json_string>() == "John Doe");
    assert(object.has_attribute("age"));
    // assert(object.contains("val")); // failure
  };

  auto empty_object = []()
  {
    {
      std::string str{R"({})"};
      auto object = parse(str).get_as<json_object>();
      assert(object.size() == 0);
    }
    {
      std::string str{R"([])"};
      auto array = parse(str).get_as<json_array>();
      assert(array.size() == 0);
    }
  };
  auto empty_string = []()
  {
    try
    {
      auto object = parse("");
    }
    catch (int r)
    {
    }
    catch (...)
    {
      assert(false);
    }
  
  };

  auto google_map_string = []()
  {
    std::string str = R"({
      "markers": [
        {
          "name": "Rixos The Palm Dubai",
          "position": [25.1212, 55.1535]
        },
        {
          "name": "Shangri-La Hotel",
          "location": [25.2084, 55.2719]
        },
        {
          "name": "Grand Hyatt",
          "location": [25.2285, 55.3273]
        }
      ]
      })";
    auto object = parse(str);
  };

  auto json_generator_com = []()
  {
    std::string str = R"([
  {
    "_id": "64aa7bdf5613f2856eb1eb9d",
    "index": 0,
    "guid": "6c92a1e5-ef45-4154-ace2-4d40ee238012",
    "isActive": false,
    "balance": "$3,392.65",
    "picture": "http://placehold.it/32x32",
    "age": 24,
    "eyeColor": "green",
    "name": "Fletcher Robinson",
    "gender": "male",
    "company": "UBERLUX",
    "email": "fletcherrobinson@uberlux.com",
    "phone": "+1 (828) 491-3942",
    "address": "642 Atlantic Avenue, Roeville, Connecticut, 7364",
    "about": "Officia occaecat culpa qui anim reprehenderit deserunt minim ad elit aliquip voluptate aliquip ea. Reprehenderit eiusmod ex reprehenderit eiusmod mollit ad. Veniam elit aliquip reprehenderit officia labore qui duis in aliquip consectetur in nisi irure esse. Pariatur aute magna id esse anim proident ex ex commodo. Ipsum ex sit occaecat nisi ex ad. Nostrud occaecat veniam duis velit quis. Commodo magna officia qui non.\r\n",
    "registered": "2015-12-05T09:53:25 -07:00",
    "latitude": -54.895103,
    "longitude": -80.360413,
    "tags": [
      "occaecat",
      "sunt",
      "esse",
      "exercitation",
      "amet",
      "dolor",
      "est"
    ],
    "friends": [
      {
        "id": 0,
        "name": "Susie Moody"
      },
      {
        "id": 1,
        "name": "Henrietta Briggs"
      },
      {
        "id": 2,
        "name": "Bridget Barrera"
      }
    ],
    "greeting": "Hello, Fletcher Robinson! You have 6 unread messages.",
    "favoriteFruit": "banana"
  },
  {
    "_id": "64aa7bdfced153ef88b88db9",
    "index": 1,
    "guid": "dfb2a804-beaa-4085-8550-da71f7d2c288",
    "isActive": true,
    "balance": "$2,965.67",
    "picture": "http://placehold.it/32x32",
    "age": 21,
    "eyeColor": "brown",
    "name": "Carol Holloway",
    "gender": "female",
    "company": "BOILICON",
    "email": "carolholloway@boilicon.com",
    "phone": "+1 (951) 581-3159",
    "address": "288 Wythe Avenue, Reinerton, Nebraska, 274",
    "about": "Cillum quis ea non eu ullamco laborum proident non voluptate quis. Deserunt officia laboris quis et voluptate consectetur commodo. Velit duis eu aute voluptate duis id Lorem dolor nisi exercitation dolor officia ad consectetur.\r\n",
    "registered": "2017-04-22T06:28:21 -07:00",
    "latitude": 69.870437,
    "longitude": -10.544068,
    "tags": [
      "velit",
      "tempor",
      "fugiat",
      "nulla",
      "fugiat",
      "veniam",
      "dolore"
    ],
    "friends": [
      {
        "id": 0,
        "name": "Carolyn Lancaster"
      },
      {
        "id": 1,
        "name": "Richardson Donovan"
      },
      {
        "id": 2,
        "name": "Esmeralda Richard"
      }
    ],
    "greeting": "Hello, Carol Holloway! You have 2 unread messages.",
    "favoriteFruit": "apple"
  },
  {
    "_id": "64aa7bdf3b912bbe6a692dfb",
    "index": 2,
    "guid": "f374d99c-098f-4b95-ac3c-17e45d131508",
    "isActive": true,
    "balance": "$3,942.68",
    "picture": "http://placehold.it/32x32",
    "age": 37,
    "eyeColor": "green",
    "name": "Beasley Saunders",
    "gender": "male",
    "company": "FUELTON",
    "email": "beasleysaunders@fuelton.com",
    "phone": "+1 (879) 462-3503",
    "address": "174 Richards Street, Southview, Palau, 6340",
    "about": "Aute velit est deserunt quis ex. Aute tempor in ipsum ea laboris nostrud nisi esse. Dolor nisi aliqua adipisicing esse ad cupidatat aliqua excepteur ipsum elit fugiat aliqua. Culpa enim esse do occaecat consectetur culpa irure. Do ex amet dolore enim est reprehenderit labore incididunt. Est irure velit sunt proident et et eu culpa ut aliquip ut irure reprehenderit. Et ea quis voluptate cupidatat cillum deserunt voluptate ut cillum cillum mollit cupidatat occaecat voluptate.\r\n",
    "registered": "2016-03-24T07:57:41 -07:00",
    "latitude": 67.751425,
    "longitude": 91.168294,
    "tags": [
      "aute",
      "commodo",
      "qui",
      "cupidatat",
      "sint",
      "ad",
      "ullamco"
    ],
    "friends": [
      {
        "id": 0,
        "name": "Church Brewer"
      },
      {
        "id": 1,
        "name": "Obrien Walton"
      },
      {
        "id": 2,
        "name": "Millie Kelley"
      }
    ],
    "greeting": "Hello, Beasley Saunders! You have 2 unread messages.",
    "favoriteFruit": "banana"
  },
  {
    "_id": "64aa7bdfb8ea51cd25c3de8b",
    "index": 3,
    "guid": "dce9fbc3-fff0-46eb-b527-5f3e2b00cb1f",
    "isActive": false,
    "balance": "$3,348.85",
    "picture": "http://placehold.it/32x32",
    "age": 23,
    "eyeColor": "green",
    "name": "Guadalupe Ellison",
    "gender": "female",
    "company": "XYQAG",
    "email": "guadalupeellison@xyqag.com",
    "phone": "+1 (976) 529-3258",
    "address": "913 Coffey Street, Groveville, District Of Columbia, 8992",
    "about": "Reprehenderit deserunt qui tempor duis in sunt occaecat sunt tempor exercitation irure labore sint. Consectetur aliquip ipsum consequat commodo minim fugiat id sint anim sit non esse commodo. Voluptate elit proident tempor Lorem. Excepteur anim ea quis consequat Lorem. Incididunt consequat enim quis qui. Duis fugiat ullamco laboris velit cillum nostrud ex non non. Eu ullamco sit eiusmod ut deserunt consequat consectetur ut.\r\n",
    "registered": "2018-05-26T12:55:19 -07:00",
    "latitude": -16.365646,
    "longitude": -38.265237,
    "tags": [
      "elit",
      "aliquip",
      "duis",
      "nisi",
      "laboris",
      "fugiat",
      "cillum"
    ],
    "friends": [
      {
        "id": 0,
        "name": "Sharp Newton"
      },
      {
        "id": 1,
        "name": "Dennis Webster"
      },
      {
        "id": 2,
        "name": "Gates Murphy"
      }
    ],
    "greeting": "Hello, Guadalupe Ellison! You have 7 unread messages.",
    "favoriteFruit": "banana"
  },
  {
    "_id": "64aa7bdfd2402459a137e91b",
    "index": 4,
    "guid": "8eaeaf3f-4751-4e06-b17c-508d9b4e8c93",
    "isActive": false,
    "balance": "$3,342.38",
    "picture": "http://placehold.it/32x32",
    "age": 30,
    "eyeColor": "blue",
    "name": "Heath Berger",
    "gender": "male",
    "company": "UNISURE",
    "email": "heathberger@unisure.com",
    "phone": "+1 (884) 550-2184",
    "address": "551 Garden Street, Grill, Maine, 9010",
    "about": "In et velit fugiat ipsum officia eiusmod voluptate irure dolore exercitation non reprehenderit. Excepteur in adipisicing aliqua consequat elit est veniam dolore cillum deserunt eu. Mollit deserunt do aliqua esse.\r\n",
    "registered": "2015-03-11T03:00:07 -07:00",
    "latitude": 53.187688,
    "longitude": -141.399625,
    "tags": [
      "Lorem",
      "id",
      "incididunt",
      "eu",
      "consequat",
      "laborum",
      "quis"
    ],
    "friends": [
      {
        "id": 0,
        "name": "Leslie Woods"
      },
      {
        "id": 1,
        "name": "Tanner Melton"
      },
      {
        "id": 2,
        "name": "Erica Hunt"
      }
    ],
    "greeting": "Hello, Heath Berger! You have 1 unread messages.",
    "favoriteFruit": "banana"
  },
  {
    "_id": "64aa7bdf713579487604dc5a",
    "index": 5,
    "guid": "b2cba665-a083-4a36-848e-f1e2470576d6",
    "isActive": false,
    "balance": "$2,796.54",
    "picture": "http://placehold.it/32x32",
    "age": 24,
    "eyeColor": "brown",
    "name": "Pate Howe",
    "gender": "male",
    "company": "MULTIFLEX",
    "email": "patehowe@multiflex.com",
    "phone": "+1 (950) 510-2896",
    "address": "528 Times Placez, Cashtown, Delaware, 9831",
    "about": "Culpa officia Lorem proident ad. Minim adipisicing ex exercitation cillum sunt sint aliquip. Velit mollit do aliqua nostrud enim et. Eu et nostrud duis excepteur nisi consectetur. Ea tempor do enim commodo consectetur qui. Laboris in duis commodo nisi laboris occaecat anim culpa quis.\r\n",
    "registered": "2014-05-01T07:52:08 -07:00",
    "latitude": 48.193379,
    "longitude": 92.750469,
    "tags": [
      "dolor",
      "ullamco",
      "commodo",
      "ipsum",
      "exercitation",
      "consectetur",
      "deserunt"
    ],
    "friends": [
      {
        "id": 0,
        "name": "Lynne Alford"
      },
      {
        "id": 1,
        "name": "Cohen Hanson"
      },
      {
        "id": 2,
        "name": "Magdalena Reid"
      }
    ],
    "greeting": "Hello, Pate Howe! You have 6 unread messages.",
    "favoriteFruit": "strawberry"
  }
])";

    auto array = parse(str);
  };

  empty_object();
  empty_string();

  normal_case();

  google_map_string();

  json_generator_com();
}

int main()
{
  test_hsjson_parser();
}
