module Snes65816

  ;; All common data registers can be extended using this class
  class Register
    macro initialize(name)
      self.name = name
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

end

;; Extends the number class to support ASM core handling
class Number

  ;; Returns whether this is a DP number or not
  macro dp?
    self.type == :direct_page
  end

  ;; Mark this number to be DP
  macro dp
    self.type = :direct_page
    self
  end

  ;; Returns whether this is a long address
  macro long_address?
    self.type == :long_address
  end

  ;; Marks this number to be a long address
  macro long_address
    self.type = :long_address
    self
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
