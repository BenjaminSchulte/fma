module SNES

  ;; The header object
  module Header
    ;; The maker code
    MAKER_CODE = $00

    ;; The game code
    GAME_CODE = ""

    ;; The title of the game
    GAME_TITLE = "UNTITLED ROM"

    ;; The version of the game
    VERSION = 0

    ;; The destination code of the game
    DESTINATION_CODE = 0

    ;; ROM mapping
    MAP_MODE = 0

    ;; ROM type
    ROM_TYPE = 0

    ;; SRAM size
    SRAM_SIZE = 0

    ;; Sets the game code
    macro game_code(code)
      self.GAME_CODE = code
    end

    ;; Sets the game title
    macro game_title(title)
      self.GAME_TITLE = title
    end

    ;; Sets the version
    macro version(version)
      self.VERSION = version
    end

    ;; Sets the maker code
    macro maker_code(code)
      self.MAKER_CODE = code
    end

    ;; Sets the destination code
    macro destination(code)
      case code
      when :japan
        self.DESTINATION_CODE = 0
      when :north_america
        self.DESTINATION_CODE = 1
      when :all_of_europe
        self.DESTINATION_CODE = 2
      when :scandinavia
        self.DESTINATION_CODE = 3
      when :france
        self.DESTINATION_CODE = 6
      when :netherlands
        self.DESTINATION_CODE = 7
      when :spain
        self.DESTINATION_CODE = 8
      when :germany
        self.DESTINATION_CODE = 9
      when :italy
        self.DESTINATION_CODE = $A
      when :china
        self.DESTINATION_CODE = $B
      when :korea
        self.DESTINATION_CODE = $D
      when :common
        self.DESTINATION_CODE = $E
      when :canada
        self.DESTINATION_CODE = $F
      when :brazil
        self.DESTINATION_CODE = $10
      when :australia
        self.DESTINATION_CODE = $11
      when :X
        self.DESTINATION_CODE = $12
      when :Y
        self.DESTINATION_CODE = $13
      when :Z
        self.DESTINATION_CODE = $14
      end
    end

    ;; Interrupt vector for CPU
    module Interrupt
      COP = nil
      IRQBRK = nil
      NMI = nil
      IRQ = nil
    end

    ;; Interrupt vector for emulated version of CPU
    module EmulatorInterrupt
      COP = nil
      NMI = nil
      RESET = nil
      IRQBRK = nil
    end

    ;; Allows to set interrupts easily
    macro on(name, callback)
      self.Interrupt[name] = callback
    end

    ;; Allows to set interrupts easily
    macro on_emulated(name, callback)
      self.EmulatorInterrupt[name] = callback
    end

    ;; Configures the memory map
    macro memory_map
      yield @MemoryConfiguration
    end

  end

  ;; The memory configuration
  module MemoryConfiguration
    ;; The bank of the heade
    HEADER_BANK = 0

    ;; The address of the heade
    HEADER_ADDRESS = 0

    ;; Configures the ROM banks
    macro banks(*args, **kwargs)
      Snes65816.configure_banks *args, **kwargs
    end

    ;; Configures the header location
    macro header_location(bank, address)
      self.HEADER_BANK = bank
      self.HEADER_ADDRESS = address
    end
  end

  ;; Configures the header for setup
  macro configure
    yield @Header

    module Header

      lo

    end
  end
end


SNES.configure do

  game_code "TEST"
  game_title "My first test"
  destination :germany

  memory_map do

    header_location $00, $FFB0

    banks $C0..$FF, address: $0000..$FFFF, located_at: $0
    banks $00..$07, address: $0000..$7FFF, shadows_banks: $C0..$C7, shadows_addresses: $8000..$FFFF

  end

end
