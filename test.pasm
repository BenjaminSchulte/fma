require "engine/header/snes"


def main

  Compiler.print "Compiling main"

  SNES.initialize
  NOP
  NOP
  NOP
  NOP

end


SNES.configure do

  game_code "TEST"
  game_title "My first test"
  destination :germany

  memory_map do

    banks $C0..$FF, address: $0000..$FFFF, located_at: $0
    banks $00..$07, address: $0000..$7FFF, shadows_banks_from: $C0, shadows_addresses_from: $8000

  end

end
