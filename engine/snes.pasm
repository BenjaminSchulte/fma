module SNES

  ;; Initializes the SNES to a safe state
  def initialize
    SEP     #$30    ; X,Y,A are 8 bit numbers
    LDA     #$8F    ; screen off, full brightness
    STA     $2100   ; brightness + screen enable register
    STZ     $2101   ; Sprite register (size + address in VRAM)
    STZ     $2102   ; Sprite registers (address of sprite memory [OAM])
    STZ     $2103   ;    ""                       ""
    STZ     $2105   ; Mode 0, = Graphic mode register
    STZ     $2106   ; noplanes, no mosaic, = Mosaic register
    STZ     $2107   ; Plane 0 map VRAM location
    STZ     $2108   ; Plane 1 map VRAM location
    STZ     $2109   ; Plane 2 map VRAM location
    STZ     $210A   ; Plane 3 map VRAM location
    STZ     $210B   ; Plane 0+1 Tile data location
    STZ     $210C   ; Plane 2+3 Tile data location
    STZ     $210D   ; Plane 0 scroll x (first 8 bits)
    STZ     $210D   ; Plane 0 scroll x (last 3 bits) #$0 - #$07ff
    LDA     #$FF    ; The top pixel drawn on the screen isn't the top one in the tilemap, it's the one above that.
    STA     $210E   ; Plane 0 scroll y (first 8 bits)
    STA     $2110   ; Plane 1 scroll y (first 8 bits)
    STA     $2112   ; Plane 2 scroll y (first 8 bits)
    STA     $2114   ; Plane 3 scroll y (first 8 bits)
    LDA     #$07    ; Since this could get quite annoying, it's better to edit the scrolling registers to fix this.
    STA     $210E   ; Plane 0 scroll y (last 3 bits) #$0 - #$07ff
    STA     $2110   ; Plane 1 scroll y (last 3 bits) #$0 - #$07ff
    STA     $2112   ; Plane 2 scroll y (last 3 bits) #$0 - #$07ff
    STA     $2114   ; Plane 3 scroll y (last 3 bits) #$0 - #$07ff
    STZ     $210F   ; Plane 1 scroll x (first 8 bits)
    STZ     $210F   ; Plane 1 scroll x (last 3 bits) #$0 - #$07ff
    STZ     $2111   ; Plane 2 scroll x (first 8 bits)
    STZ     $2111   ; Plane 2 scroll x (last 3 bits) #$0 - #$07ff
    STZ     $2113   ; Plane 3 scroll x (first 8 bits)
    STZ     $2113   ; Plane 3 scroll x (last 3 bits) #$0 - #$07ff
    LDA     #$80    ; increase VRAM address after writing to $2119
    STA     $2115   ; VRAM address increment register
    STZ     $2116   ; VRAM address low
    STZ     $2117   ; VRAM address high
    STZ     $211A   ; Initial Mode 7 setting register
    STZ     $211B   ; Mode 7 matrix parameter A register (low)
    LDA     #$01
    STA     $211B   ; Mode 7 matrix parameter A register (high)
    STZ     $211C   ; Mode 7 matrix parameter B register (low)
    STZ     $211C   ; Mode 7 matrix parameter B register (high)
    STZ     $211D   ; Mode 7 matrix parameter C register (low)
    STZ     $211D   ; Mode 7 matrix parameter C register (high)
    STZ     $211E   ; Mode 7 matrix parameter D register (low)
    STA     $211E   ; Mode 7 matrix parameter D register (high)
    STZ     $211F   ; Mode 7 center position X register (low)
    STZ     $211F   ; Mode 7 center position X register (high)
    STZ     $2120   ; Mode 7 center position Y register (low)
    STZ     $2120   ; Mode 7 center position Y register (high)
    STZ     $2121   ; Color number register ($0-ff)
    STZ     $2123   ; BG1 & BG2 Window mask setting register
    STZ     $2124   ; BG3 & BG4 Window mask setting register
    STZ     $2125   ; OBJ & Color Window mask setting register
    STZ     $2126   ; Window 1 left position register
    STZ     $2127   ; Window 2 left position register
    STZ     $2128   ; Window 3 left position register
    STZ     $2129   ; Window 4 left position register
    STZ     $212A   ; BG1, BG2, BG3, BG4 Window Logic register
    STZ     $212B   ; OBJ, Color Window Logic Register (or,and,xor,xnor)
    STA     $212C   ; Main Screen designation (planes, sprites enable)
    STZ     $212D   ; Sub Screen designation
    STZ     $212E   ; Window mask for Main Screen
    STZ     $212F   ; Window mask for Sub Screen
    LDA     #$30
    STA     $2130   ; Color addition & screen addition init setting
    STZ     $2131   ; Add/Sub sub designation for screen, sprite, color
    LDA     #$E0
    STA     $2132   ; color data for addition/subtraction
    STZ     $2133   ; Screen setting (interlace x,y/enable SFX data)
    STZ     $4200   ; Enable V-blank, interrupt, Joypad register
    LDA     #$FF
    STA     $4201   ; Programmable I/O port
    STZ     $4202   ; Multiplicand A
    STZ     $4203   ; Multiplier B
    STZ     $4204   ; Multiplier C
    STZ     $4205   ; Multiplicand C
    STZ     $4206   ; Divisor B
    STZ     $4207   ; Horizontal Count Timer
    STZ     $4208   ; Horizontal Count Timer MSB (most significant bit)
    STZ     $4209   ; Vertical Count Timer
    STZ     $420A   ; Vertical Count Timer MSB
    STZ     $420B   ; General DMA enable (bits 0-7)
    STZ     $420C   ; Horizontal DMA (HDMA) enable (bits 0-7)
    STZ     $420D     ; Access cycle designation (slow/fast rom)
    CLI
  end

end
