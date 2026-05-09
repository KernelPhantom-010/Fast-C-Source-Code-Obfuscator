# Fast C Source Code Obfuscator

A lightweight experimental C source code obfuscator written in C.

This tool currently supports:

* Function name obfuscation
* XOR string obfuscation
* Runtime string decryption
* Simple source rewriting

---

# Features

## Function Obfuscation

Functions are automatically renamed to randomized `_0x...` identifiers.

Example:

```c
int main()
```

becomes:

```c
int _0xA92KJ()
```

---

## String Obfuscation

Strings are XOR encrypted using a static XOR key.

Example:

```c
char* message_vars = "Hello";
```

becomes:

```c
char message_vars[] = {0xB7,0x9A,0x93,0x93,0x90,0x00};
_0xDECRYPT(message_vars);
```

Strings are decrypted at runtime.

---

# Usage

```bash
obfuscater.exe file.c
```

Example:

```bash
obfuscater.exe test.c
```

---

# IMPORTANT RULES

The obfuscator currently uses simple text parsing and replacement.

Because of this, there are several important limitations you MUST follow.

---

# 1. Use Pointer Strings

Always declare strings using pointers:

## Correct

```c
char* text_vars = "Hello";
```

## Wrong

```c
char text_vars[] = "Hello";
```

The current parser expects pointer-style declarations.

---

# 2. Every Obfuscated String MUST Contain `_vars`

Only variables ending with `_vars` are selected for string obfuscation.

## Correct

```c
char* password_vars = "secret";
```

## Wrong

```c
char* password = "secret";
```

If `_vars` is missing, the string will be ignored.

---

# 3. Be Careful With Strings Inside Function Calls

Example:

```c
printf("Hello");
```

Function argument strings are intentionally skipped.

However:

Some edge cases may still accidentally get obfuscated depending on formatting.

Especially strings near:

* nested parentheses
* macros
* multiline expressions
* commas
* unusual formatting

Be careful when using complex inline strings.

---

# 4. Avoid Strings Containing Complex Parentheses Patterns

Some patterns may confuse the parser.

Examples:

```c
char* text_vars = "test()";
```

or:

```c
char* text_vars = "(hello)";
```

These may occasionally produce incorrect replacements depending on source layout.

---

# 5. Always Keep Backup Copies

The obfuscator rewrites the source file directly.

Always keep backups of your original source code.

Recommended workflow:

```text
original.c -> obfuscater -> obfuscated.c
```

Do NOT repeatedly obfuscate already obfuscated files.

---

# How String Encryption Works

Each character is XOR encrypted using:

```c
0xFF
```

Encryption:

```c
encrypted = original ^ 0xFF
```

Decryption:

```c
original = encrypted ^ 0xFF
```

Because XOR is reversible, the same operation decrypts the string.

---

# Example

## Original

```c
#include <stdio.h>

int main()
{
    char* message_vars = "Hello";
    printf("Test");
}
```

## Obfuscated

```c
void _0xDECRYPT(char* str) {
    for (int i = 0; str[i] != 0; i++) {
        str[i] ^= 0xFF;
    }
}

#define _0xA81K main

#include <stdio.h>

int _0xA81K()
{
    char* message_vars = {0xB7,0x9A,0x93,0x93,0x90,0x00};
    _0xDECRYPT(message_vars);

    printf("Test");
}
```

---

# Known Limitations

This project is still experimental.

Current limitations:

* No real C parser
* Uses string replacement internally
* Macros may break parsing
* Complex declarations may fail
* Nested expressions may fail
* Duplicate strings can cause replacement issues
* Function pointers are unsupported
* Multiline string edge cases may occur

---

# Recommended Usage

Best used for:

* Small C projects
* Learning purposes
* Basic anti-static-analysis
* Lightweight source obfuscation

Not recommended for:

* Production-grade protection
* Advanced malware protection
* Commercial DRM systems
* Complex compiler pipelines

---

# Disclaimer

This tool is experimental.

Always verify generated code manually before compiling or distributing it.

Use at your own risk.
