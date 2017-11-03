module CPU
  ;; IRQ disable flag
  FLAG_I = $04

  ;; Decimal flag
  FLAG_D = $08

  ;; Index register flag
  FLAG_X = $10

  ;; Accumulator flag
  FLAG_M = $20

  ;; Lower scope
  scope :_scope_low, bank: $00, at: $2180

  ;; WRAM data register
  declare :WMDATA, as: :uint8, in: _scope_low

  ;; WRAM address
  declare :WMADDR, as: :uint24, in: _scope_low


  ;; Higher scope
  scope :_scope_high, bank: $00, at: $4200

  ;; Interrupt enable
  declare :NMITIMEN, as: :uint8, in: _scope_high

  ;; IO Port Write
  declare :WRIO, as: :uint8, in: _scope_high

  ;; Multiplicand
  declare :WRMPYA, as: :uint8, in: _scope_high

  ;; Multiplicand
  declare :WRMPYB, as: :uint8, in: _scope_high

  ;; Divisor & Dividend
  declare :WRDIVL, as: :uint8, in: _scope_high

  ;; Divisor & Dividend
  declare :WRDIVH, as: :uint8, in: _scope_high

  ;; Divisor & Dividend
  declare :WRDIVB, as: :uint8, in: _scope_high

  ;; IRQ Timer register (Horizontal)
  declare :HTIME, as: :uint16, in: _scope_high

  ;; IRQ Timer register (Vertical)
  declare :VTIME, as: :uint16, in: _scope_high

  ;; DMA enable
  declare :MDMAEN, as: :uint8, in: _scope_high

  ;; HDMA enable
  declare :HDMAEN, as: :uint8, in: _scope_high

  ;; ROM speed
  declare :MEMSEL, as: :uint8, in: _scope_high

  ;; Debug
  declare :DEBUG, as: :uint16, in: _scope_high

  ;; Interrupt flag
  declare :RDNMI, as: :uint8, in: _scope_high

  ;; Interrupt flag
  declare :TIMEUP, as: :uint8, in: _scope_high

  ;; PPU Status
  declare :HVBJOY, as: :uint8, in: _scope_high

  ;; IO Port read
  declare :RDIO, as: :uint8, in: _scope_high

  ;; Multiplion or divide result
  declare :RDDIV, as: :uint16, in: _scope_high

  ;; Multiplion or divide result
  declare :RDMPY, as: :uint16, in: _scope_high

  ;; Joypad
  declare :JOY, as: :uint16, in: _scope_high, length: 4


end
