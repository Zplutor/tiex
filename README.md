## Overview
Tiex is a C++ library that used to format date time. Unlike other date time format libraries, tiex uses a simple expression to specify format rules, and examines these rules according to the difference between two times.

## Quick Start
Here is a quick start guide to tiex. Before using tiex, we should prepare three arguments: format expression, referenced time and formatted time.

* Format expression, specifies one or more format rules. A rule consists of a condition and a result. The result would be used to generate output if referenced time and formatted time match the condition.
* Referenced time, the time that compares to the formatted time.
* Formatted time, the time that is being formatted.

Firstly, we create a formatter with format expression:
```cpp
std::string expression = 
    "[-1~min,0]{Just now}"
    "[-1~h,0]{%H:%M:%S}"
    "[-2.d,-1.d]{%~h hours}";

tiex::Formatter formatter = tiex::Formatter::Create(expression);
```

There are three rules in this expression. Each rule has a condition represented by a pair of brackets and a result represented by a pair of braces. Explaination of these rules:

1. If formatted time is earlier than referenced time in one minute, then output "Just now".
2. If formatted time is earlier than referenced time in one hour, then output with format "HH:MM:SS".
3. If formatted time is earlier than referenced time from the first previous day to the second previous day, then output "N hours", which "N" is the number of hours between formatted time and referenced time.

Next, we can format times with this formatter. The formatter would examines every rule until a matched rule is found. As the code shows below:
```cpp
std::time_t referenced_time = 1518070016;  //2018-02-08 14:06:56

std::time_t target_time = 1518069960;  //2018-02-08 14:06:00
std::cout << formatter.Format(referenced_time, target_time) << std::endl;  //Output: Just now

target_time = 1518066451;  //2018-02-08 13:07:31
std::cout << formatter.Format(referenced_time, target_time) << std::endl;  //Output: 13:07:31

target_time = 1518012416;  //2018-02-07 22:06:56
std::cout << formatter.Format(referenced_time, target_time) << std::endl;  //Output: 16 hours
```

## More Information
* [Syntax](https://github.com/Zplutor/tiex/wiki/Syntax) of the expression.
* The choice between [Char and Wide Char](https://github.com/Zplutor/tiex/wiki/Char-and-Wide-Char).
* How to use [Custom Localization](https://github.com/Zplutor/tiex/wiki/Custom-Localization).
