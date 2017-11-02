; Extends the memory manager for some core functions
class CompilerMemoryManager
  macro allow(bank=nil, at=nil, range=nil)
    Compiler.print "allow MemoryManager ", bank, " ", at, " ", range
  end
end

; Extends the compiler scope
class CompilerScope
  macro on_enter_function(function)
    Compiler.print "Entering function"
  end
end

module Snes65816

  opcodes = {}

  RAM = CompilerMemoryManager.new
  ROM = CompilerMemoryManager.new
  Compiler.register_static_code ROM

  ;; All common data registers can be extended using this class
  class Register
    macro initialize(name)
      self.name = name
      self.register_size = 8
    end

    macro set_size(size)
      if self.Y?
        X.register_size = size
      else
        self.register_size = size
      end
    end

    macro size
      if self.Y?
        X.register_size
      else
        self.register_size
      end
    end

    macro A?
      self.name == "A"
    end

    macro X?
      self.name == "X"
    end

    macro Y?
      self.name == "Y"
    end

    macro S?
      self.name == "S"
    end
  end

  ;; Invokes an opcode
  macro call_opcode(name, left=nil, right=nil)
    call_type = nil
    call_arguments = []

    if left.nil?
      raise "Unexpected parameter type"
    elsif right.nil?
      call_arguments = [left]

      if left.is_a? TypedNumber
        case left.type
        when :constant
          call_type = :im
        else
          raise "Unexpected parameter type: ", left.type
        end
      else
        raise "Unexpected parameter type"
      end
    else
      raise "Unexpected parameter type"
    end

    raise "Opcode #{name} does not support parameter type #{call_type}" unless Snes65816.opcodes[name].key? call_type

    config = Snes65816.opcodes[name][call_type]

    Compiler.print "!!!TODO!!! CALL ", config
  end

  ;; Registers a new operator
  macro operator(opcode, name, type, &block)
    name = name.upcase
    type = type.downcase

    macro ::\{name} *args
      call_opcode name, *args
    end

    Snes65816.opcodes[name] = {} unless Snes65816.opcodes.key? name
    Snes65816.opcodes[name][type] = {
      opcode: opcode,
      block: block
    }
  end

  ;; Configures the ROM
  macro configure_banks(banks, address, shadows_banks_from=nil, shadows_addresses_from=nil, located_at=nil)
    banks.each do |bank|
      unless shadows_addresses_from.nil? && shadows_banks_from.nil?
        child = ROM.allocate
      else
        child = ROM.allocate_shadow
      end

      child.allow bank: bank, range: address
      child.location = located_at

      unless shadows_addresses_from.nil?
        child.address_add = shadows_addresses_from - address.first
      end

      unless shadows_banks_from.nil?
        child.address_and = $FFFF
        child.address_or  = shadows_banks_from << 16

        shadows_banks_from += 1
      end
    end
  end

  ;; Assigns the memory location to the scope
  macro locate_at(bank=nil, address=nil)
    range_from = nil
    range_to = nil

    unless bank.nil? && address.nil?
      Compiler.print "TODO: locate_at"
    end

    unless range_from.nil?
      Compiler.current_scope.locate_at range_from..range_to
    end
  end
end

;; Allocates a new RAM scope
macro scope(name, bank=nil, at=nil, length=nil, in=nil, shadows_bank=nil)
  address_range = nil

  if in.nil?
    ram = Snes65816::RAM.allocate
  else
    ram = in.allocate
  end

  unless bank.nil? && at.nil?
    ram.allow bank: bank, at: at
  end

  unless length.nil?
    ram.size = length
  end

  unless shadows_bank.nil?
    ram.address_and = $FFFF
    ram.address_or  = shadows_bank << 16
  end

  ram.dump

  callee[name] = ram
  ram
end

;; Extends the number class to support ASM core handling
class Number

  ;; Mark this number to be DP
  macro dp
    TypedNumber.new self, :direct_page
  end

  ;; Marks this number to be a long address
  macro long_address
    TypedNumber.new self, :long_address
  end

  ;; Returns the address of this number without the bank
  macro address
    self & $FFFF
  end

  ;; Returns the bank of this number
  macro bank
    (self >> 16) & $FF
  end

end

;; The accumulator
A = Snes65816::Register.new "A"

;; The index register X
X = Snes65816::Register.new "X"

;; The index register Y
Y = Snes65816::Register.new "Y"

;; The stack register
S = Snes65816::Register.new "S"

;; Support dp(number) syntax
macro dp(number)
  number.dp
  number
end

;; Decorator for location
macro locate_at(**kwargs)
  Compiler.each_function do
    Snes65816.locate_at **kwargs

    yield
  end
end

;; Memory block declaration
macro memory_block(**kwargs)
  Compiler.define do
    Snes65816.locate_at **kwargs

    yield Compiler.current_scope
  end
end

Snes65816.operator $00, :BRK, :impl
Snes65816.operator $01, :ora, :IDPX
Snes65816.operator $02, :cop, :IM
Snes65816.operator $03, :ora, :SR
Snes65816.operator $04, :tsb, :DP
Snes65816.operator $05, :ora, :DP
Snes65816.operator $06, :asl, :DP
Snes65816.operator $07, :ora, :ILDP
Snes65816.operator $0a, :asl, :IMPL
Snes65816.operator $0b, :phd, :IMPL
Snes65816.operator $0c, :tsb, :ADDR
Snes65816.operator $0d, :ora, :ADDR
Snes65816.operator $0e, :asl, :ADDR
Snes65816.operator $0f, :ora, :LONG
Snes65816.operator $10, :bpl, :RELB
Snes65816.operator $11, :ora, :IDPY
Snes65816.operator $12, :ora, :IDP
Snes65816.operator $13, :ora, :ISRY
Snes65816.operator $14, :trb, :DP
Snes65816.operator $15, :ora, :DPX
Snes65816.operator $16, :asl, :DPX
Snes65816.operator $17, :ora, :ILDPY
Snes65816.operator $18, :clc, :IMPL
Snes65816.operator $19, :ora, :ADDRY
Snes65816.operator $1a, :inc, :IMPL
Snes65816.operator $1b, :tcs, :IMPL
Snes65816.operator $1c, :trb, :ADDR
Snes65816.operator $1d, :ora, :ADDRX
Snes65816.operator $1e, :asl, :ADDRX
Snes65816.operator $1f, :ora, :LONGX
Snes65816.operator $20, :jsr, :ADDR_PC
Snes65816.operator $21, :and, :IDPX
Snes65816.operator $22, :jsl, :LONG
Snes65816.operator $23, :and, :SR
Snes65816.operator $24, :bit, :DP
Snes65816.operator $25, :and, :DP
Snes65816.operator $26, :rol, :DP
Snes65816.operator $27, :and, :ILDP
Snes65816.operator $2a, :rol, :IMPL
Snes65816.operator $2b, :pld, :IMPL
Snes65816.operator $2c, :bit, :ADDR
Snes65816.operator $2d, :and, :ADDR
Snes65816.operator $2e, :rol, :ADDR
Snes65816.operator $2f, :and, :LONG
Snes65816.operator $30, :bmi, :RELB
Snes65816.operator $31, :and, :IDPY
Snes65816.operator $32, :and, :IDP
Snes65816.operator $33, :and, :ISRY
Snes65816.operator $34, :bit, :DPX
Snes65816.operator $35, :and, :DPX
Snes65816.operator $36, :rol, :DPX
Snes65816.operator $37, :and, :ILDPY
Snes65816.operator $38, :sec, :IMPL
Snes65816.operator $39, :and, :ADDRY
Snes65816.operator $3a, :dec, :IMPL
Snes65816.operator $3b, :tsc, :IMPL
Snes65816.operator $3c, :bit, :ADDRX
Snes65816.operator $3d, :and, :ADDRX
Snes65816.operator $3e, :rol, :ADDRX
Snes65816.operator $3f, :and, :LONGX
Snes65816.operator $41, :eor, :IDPX
Snes65816.operator $42, :wdm, :IM
Snes65816.operator $43, :eor, :SR
Snes65816.operator $44, :mvp, :MV
Snes65816.operator $45, :eor, :DP
Snes65816.operator $46, :lsr, :DP
Snes65816.operator $47, :eor, :ILDP
Snes65816.operator $48, :pha, :IMPL
Snes65816.operator $4a, :lsr, :IMPL
Snes65816.operator $4b, :phk, :IMPL
Snes65816.operator $4d, :eor, :ADDR
Snes65816.operator $4e, :lsr, :ADDR
Snes65816.operator $4f, :eor, :LONG
Snes65816.operator $50, :bvc, :RELB
Snes65816.operator $51, :eor, :IDPY
Snes65816.operator $52, :eor, :IDP
Snes65816.operator $53, :eor, :ISRY
Snes65816.operator $54, :mvn, :MV
Snes65816.operator $55, :eor, :DPX
Snes65816.operator $56, :lsr, :DPX
Snes65816.operator $57, :eor, :ILDPY
Snes65816.operator $58, :cli, :IMPL
Snes65816.operator $59, :eor, :ADDRY
Snes65816.operator $5a, :phy, :IMPL
Snes65816.operator $5b, :tcd, :IMPL
Snes65816.operator $5d, :eor, :ADDRX
Snes65816.operator $5e, :lsr, :ADDRX
Snes65816.operator $5f, :eor, :LONGX
Snes65816.operator $61, :adc, :IDPX
Snes65816.operator $62, :per, :RELW
Snes65816.operator $63, :adc, :SR
Snes65816.operator $64, :stz, :DP
Snes65816.operator $65, :adc, :DP
Snes65816.operator $66, :ror, :DP
Snes65816.operator $67, :adc, :ILDP
Snes65816.operator $68, :pla, :IMPL
Snes65816.operator $6a, :ror, :IMPL
Snes65816.operator $6d, :adc, :ADDR
Snes65816.operator $6e, :ror, :ADDR
Snes65816.operator $6f, :adc, :LONG
Snes65816.operator $70, :bvs, :RELB
Snes65816.operator $71, :adc, :IDPY
Snes65816.operator $72, :adc, :IDP
Snes65816.operator $73, :adc, :ISRY
Snes65816.operator $74, :stz, :DPX
Snes65816.operator $75, :adc, :DPX
Snes65816.operator $76, :ror, :DPX
Snes65816.operator $77, :adc, :ILDPY
Snes65816.operator $78, :sei, :IMPL
Snes65816.operator $79, :adc, :ADDRY
Snes65816.operator $7a, :ply, :IMPL
Snes65816.operator $7b, :tdc, :IMPL
Snes65816.operator $7d, :adc, :ADDRX
Snes65816.operator $7e, :ror, :ADDRX
Snes65816.operator $7f, :adc, :LONGX
Snes65816.operator $81, :sta, :IDPX
Snes65816.operator $83, :sta, :SR
Snes65816.operator $84, :sty, :DP
Snes65816.operator $85, :sta, :DP
Snes65816.operator $86, :stx, :DP
Snes65816.operator $87, :sta, :ILDP
Snes65816.operator $88, :dey, :IMPL
Snes65816.operator $8a, :txa, :IMPL
Snes65816.operator $8b, :phb, :IMPL
Snes65816.operator $8c, :sty, :ADDR
Snes65816.operator $8d, :sta, :ADDR
Snes65816.operator $8e, :stx, :ADDR
Snes65816.operator $8f, :sta, :LONG
Snes65816.operator $90, :bcc, :RELB
Snes65816.operator $91, :sta, :IDPY
Snes65816.operator $92, :sta, :IDP
Snes65816.operator $93, :sta, :ISRY
Snes65816.operator $94, :sty, :DPX
Snes65816.operator $95, :sta, :DPX
Snes65816.operator $96, :stx, :DPY
Snes65816.operator $97, :sta, :ILDPY
Snes65816.operator $98, :tya, :IMPL
Snes65816.operator $99, :sta, :ADDRY
Snes65816.operator $9a, :txs, :IMPL
Snes65816.operator $9b, :txy, :IMPL
Snes65816.operator $9c, :stz, :ADDR
Snes65816.operator $9d, :sta, :ADDRX
Snes65816.operator $9e, :stz, :ADDRX
Snes65816.operator $9f, :sta, :LONGX
Snes65816.operator $a1, :lda, :IDPX
Snes65816.operator $a2, :LDX, :im do |value|
  Compiler.print "TODO: ", value
end
Snes65816.operator $a3, :lda, :SR
Snes65816.operator $a4, :ldy, :DP
Snes65816.operator $a5, :lda, :DP
Snes65816.operator $a6, :ldx, :DP
Snes65816.operator $a7, :lda, :ILDP
Snes65816.operator $a8, :tay, :IMPL
Snes65816.operator $aa, :tax, :IMPL
Snes65816.operator $ab, :plb, :IMPL
Snes65816.operator $ac, :ldy, :ADDR
Snes65816.operator $ad, :lda, :ADDR
Snes65816.operator $ae, :ldx, :ADDR
Snes65816.operator $af, :lda, :LONG
Snes65816.operator $b0, :bcs, :RELB
Snes65816.operator $b1, :lda, :IDPY
Snes65816.operator $b2, :lda, :IDP
Snes65816.operator $b3, :lda, :ISRY
Snes65816.operator $b4, :ldy, :DPX
Snes65816.operator $b5, :lda, :DPX
Snes65816.operator $b6, :ldx, :DPY
Snes65816.operator $b7, :lda, :ILDPY
Snes65816.operator $b8, :clv, :IMPL
Snes65816.operator $b9, :lda, :ADDRY
Snes65816.operator $ba, :tsx, :IMPL
Snes65816.operator $bb, :tyx, :IMPL
Snes65816.operator $bc, :ldy, :ADDRX
Snes65816.operator $bd, :lda, :ADDRX
Snes65816.operator $be, :ldx, :ADDRY
Snes65816.operator $bf, :lda, :LONGX
Snes65816.operator $c1, :cmp, :IDPX
Snes65816.operator $c2, :REP, :im do |value|
  Compiler.print "TODO: ", value
end
Snes65816.operator $c3, :cmp, :SR
Snes65816.operator $c4, :cpy, :DP
Snes65816.operator $c5, :cmp, :DP
Snes65816.operator $c6, :dec, :DP
Snes65816.operator $c7, :cmp, :ILDP
Snes65816.operator $c8, :iny, :IMPL
Snes65816.operator $ca, :dex, :IMPL
Snes65816.operator $cb, :wai, :IMPL
Snes65816.operator $cc, :cpy, :ADDR
Snes65816.operator $cd, :cmp, :ADDR
Snes65816.operator $ce, :dec, :ADDR
Snes65816.operator $cf, :cmp, :LONG
Snes65816.operator $d0, :bne, :RELB
Snes65816.operator $d1, :cmp, :IDPY
Snes65816.operator $d2, :cmp, :IDP
Snes65816.operator $d3, :cmp, :ISRY
Snes65816.operator $d4, :pei, :DP
Snes65816.operator $d5, :cmp, :DPX
Snes65816.operator $d6, :dec, :DPX
Snes65816.operator $d7, :cmp, :ILDPY
Snes65816.operator $d8, :cld, :IMPL
Snes65816.operator $d9, :cmp, :ADDRY
Snes65816.operator $da, :phx, :IMPL
Snes65816.operator $db, :stp, :IMPL
Snes65816.operator $dd, :cmp, :ADDRX
Snes65816.operator $de, :dec, :ADDRX
Snes65816.operator $df, :cmp, :LONGX
Snes65816.operator $e1, :sbc, :IDPX
Snes65816.operator $e3, :sbc, :SR
Snes65816.operator $e4, :cpx, :DP
Snes65816.operator $e5, :sbc, :DP
Snes65816.operator $e6, :inc, :DP
Snes65816.operator $e7, :sbc, :ILDP
Snes65816.operator $e8, :inx, :IMPL
Snes65816.operator $ea, :nop, :IMPL
Snes65816.operator $eb, :xba, :IMPL
Snes65816.operator $ec, :cpx, :ADDR
Snes65816.operator $ed, :sbc, :ADDR
Snes65816.operator $ee, :inc, :ADDR
Snes65816.operator $ef, :sbc, :LONG
Snes65816.operator $f0, :beq, :RELB
Snes65816.operator $f1, :sbc, :IDPY
Snes65816.operator $f2, :sbc, :IDP
Snes65816.operator $f3, :sbc, :ISRY
Snes65816.operator $f4, :pea, :IM
Snes65816.operator $f5, :sbc, :DPX
Snes65816.operator $f6, :inc, :DPX
Snes65816.operator $f7, :sbc, :ILDPY
Snes65816.operator $f8, :sed, :IMPL
Snes65816.operator $f9, :sbc, :ADDRY
Snes65816.operator $fa, :plx, :IMPL
Snes65816.operator $fb, :xce, :IMPL
Snes65816.operator $fc, :jsr, :IADDRX
Snes65816.operator $fd, :sbc, :ADDRX
Snes65816.operator $fe, :inc, :ADDRX
Snes65816.operator $ff, :sbc, :LONGX
