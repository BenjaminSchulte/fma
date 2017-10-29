PASM
====

PASM (working title) is a new multi platform cross assembler, mainly used for SNES development.

PASM speeds up development by adding lots of macro features, utilities and improved readability to your project. It allows you to write games in a ruby like syntax, but still keep full control over your application.


Warning
-------

This project is still in the very, very alpha version! Core features might change
without any backwards compatibility.


Features
--------

* **Ruby like syntax** for assembler projects
* Manage code in **modules**, **macros** and **classes**
* Auto generated locations for variables
* Memory ranges can be reused for other variables
* Automatically keeps track about size of A, X and Y registers
* Pass numbers, strings or even registers to macros
* **Other assemblers** can be implemented, even if they have totally different opcodes


Installing
----------

This part is not yet finished, because the language is not even finished, yet. :<


Code example
------------

Simple example of macro usage:

```ruby
module SNES
  macro set_screen_brightness(brightness, enabled=true)
    LDA   #brightness | (enabled ? 0 : $80) unless brightness.A?
    STA   $2100
  end
end

def main
  SNES.set_screen_brightness enabled: false

  do_initialize_stuff

  LDA   #0
loop:
  SNES.set_screen_brightness A
  do_something
  INA
  CMP   #16
  BNE   loop

  do_something_else
end
```
