<!--
title: Logging
description: How to use the atta logging functions to print variable, vectors, colors, etc
image: https://storage.googleapis.com/atta-images/docs/tutorial/logging/logging.png
-->
# Logging

<div align="center">
  <img src="https://storage.googleapis.com/atta-images/docs/tutorial/logging/logging.png" height="400">
</div>

## Log functions
When printing using atta log functions, you can choose one of 5 printing modes:

 - `LOG_VERBOSE()`
 - `LOG_DEBUG()`
 - `LOG_SUCCESS()`
 - `LOG_WARN()`
 - `LOG_ERROR()`

Each one will be printed with a different color.

Printing with each mode:
```cpp
// Logging modes
LOG_VERBOSE("Project", "Verbose Hello World!");
LOG_DEBUG("Project", "Debug Hello World!");
LOG_SUCCESS("Project", "Success Hello World!");
LOG_WARN("Project", "Warn Hello World!");
LOG_ERROR("Project", "Error Hello World!");
```

You can also print values and variables:
```cpp
LOG_DEBUG("Project", "PI: $0, Answer: $1", 3.141592f, 42);
```
It is also possible to change the text color:
```cpp
LOG_DEBUG("Project", "[w]Hello [r]World!");
```

The available colors are:
```
[w]  -> white
[r]  -> red
[g]  -> green
[b]  -> blue
[c]  -> cyan
[m]  -> magenta
[y]  -> yellow
[k]  -> black
[*w]  -> bold white
[*r]  -> bold red
[*g]  -> bold green
[*b]  -> bold blue
[*c]  -> bold cyan
[*m]  -> bold magenta
[*y]  -> bold yellow
[*k]  -> bold black
[]   -> reset to default color
```

Check the source code for more examples :)
