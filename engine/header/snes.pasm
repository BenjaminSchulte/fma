;; The main memory
module Memory
  STACK_SIZE = $400

  ;; The RAM
  scope :RAM
  RAM.allow bank: $7E, range: $0000..$FFFF
  RAM.allow bank: $7F, range: $0000..$FFFF

  ;; RAM accessible by addressing bank 0
  scope :QuickPage, in: RAM, bank: $0, at: $0000, length: $2000, shadows_bank: $7E

  ;; The default direct page
  scope :DirectPage, in: QuickPage, at: $0000, length: $100

  ;; The stack
  scope :stack, in: QuickPage, length: STACK_SIZE
end

;; The SNES module
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

    ;; The expansion RAM size
    EXPANSION_RAM_SIZE = 0

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

    ;; Sets the map mode
    macro map_mode(code)
      self.MAP_MODE = code
    end

    ;; Sets the map mode
    macro rom_type(ram=false, battery=false)
      value = $00
      if battery && ram
        value |= $02
      elsif ram
        value |= $01
      end
      self.ROM_TYPE = value
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
      IRQBRK = nil
      NMI = nil
      IRQ = nil
    end
    Interrupt.COP = nil

    ;; Interrupt vector for emulated version of CPU
    module EmulatorInterrupt
      NMI = nil
      RESET = nil
      IRQBRK = nil
    end
    EmulatorInterrupt.COP = nil

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
    HEADER_ADDRESS = $FFB0

    ;; Configures the header location
    macro header_location(bank, address)
      HEADER_BANK = bank
      HEADER_ADDRESS = address
    end

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

  ;; Writes an interrupt to the block
  macro _write_interrupt(block, target, fallback)
    if target.nil?
      block.dw fallback
    else
      block.dw target
    end
  end

  ;; Configures the header for setup
  macro configure
    yield @Header

    module ::Header
      locate_at bank: 0

      ;; The core initialize routine which initializes the application
      def main
        A.set_size 8
        X.set_size 8

        SEI
        CLC
        XCE

        REP   #$18
        LDX   #Memory.stack + Memory.STACK_SIZE - 1
        TXS

        LDA   #1
        STA   CPU.MEMSEL

        JML   ::main
      end

      ;; Empty interrupt
      def no_interrupt
        RTI
      end

      memory_block name: ".rom_header_location", bank: MemoryConfiguration.HEADER_BANK, address: MemoryConfiguration.HEADER_ADDRESS do |block|
        ; ROM registration data
        block.dw Header.MAKER_CODE
        block.db Header.GAME_CODE, length: 4, fill: $20
        block.db 0, length: 7
        block.db 0
        block.db 0
        block.db 0

        ; ROM specifications
        block.db Header.GAME_TITLE, length: 21, fill: $20
        block.db Header.MAP_MODE
        block.db Header.ROM_TYPE
        block.db 9 ; ROM size is auto generated later
        block.db Header.EXPANSION_RAM_SIZE
        block.db Header.DESTINATION_CODE
        block.db $33
        block.db Header.VERSION

        ; Checksum is autogenerated later
        block.dw $0000
        block.dw $ffff

        ; Interrupt
        block.dw 0
        block.dw 0
        _write_interrupt block, Header::Interrupt.COP, no_interrupt
        _write_interrupt block, Header::Interrupt.IRQBRK, no_interrupt
        block.dw 0
        _write_interrupt block, Header::Interrupt.NMI, no_interrupt
        block.dw 0
        _write_interrupt block, Header::Interrupt.IRQ, no_interrupt

        ; Interrupt block 2
        block.dw 0
        block.dw 0
        _write_interrupt block, Header::EmulatorInterrupt.COP, no_interrupt
        block.dw 0
        block.dw 0
        _write_interrupt block, Header::EmulatorInterrupt.NMI, no_interrupt
        _write_interrupt block, Header::EmulatorInterrupt.RESET, main
        _write_interrupt block, Header::EmulatorInterrupt.IRQBRK, no_interrupt

      end
    end
  end
end

locate_at bank: $C0
