require "engine/header/snes"


def main

  SNES.initialize
  NOP
  NOP
  NOP
  NOP

loop:
  BRA   loop

end


SNES.configure do

  game_code "TEST"
  game_title "My first test"
  destination :germany

  map_mode $31
  rom_type ram: true, battery: true

  memory_map do

    header_location $C0, $FFB0

    banks $C0..$FF, address: $0000..$FFFF, located_at: $0
    banks $00..$07, address: $8000..$FFFF, shadows_banks_from: $C0, shadows_addresses_from: $8000

  end

end
