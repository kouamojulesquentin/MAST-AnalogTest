graph
[
   hierarchic 1 directed 1 label "DUT"
   node [ id 0 graphics [ type "octagon" fill "#10FFFF" w 90 h 43 ] LabelGraphics [ text "TAP" fontSize 13 fontStyle "bold" fontName "Lucida Console"] ]
   node [ id 1 graphics [ type "rectangle" fill "#59FF20" w 57 h 35 ] LabelGraphics [ text "TAP_IR" fontSize 13 fontStyle "bold" fontName "Lucida Console"] ]
   node [ id 2 graphics [ type "trapezoid" fill "#FF3060" outlineStyle "dashed" w 135 h 43 ] LabelGraphics [ text "TAP_DR_Mux" fontSize 13 fontStyle "bold" fontName "Lucida Console"] ]
   node [ id 2147483647 graphics [ type "rectangle" fill "#E8E8E8" w 209 h 270 ] LabelGraphics [ text "Selector :1:
Kind: Table_Based
Can_select_none: false
Reversed_order:  false
Inverted_bits:   false

Selection Table:
  [0] 0b1111
  [1] 0b1111
  [2] 0b0001

Deselection Table:
  [0] 0b1111
  [1] 0b1111
  [2] 0b1111" fontSize 13 fontStyle "bold" fontName "Lucida Console"] ]
   node [ id 3 graphics [ type "rectangle" fill "#59FF20" w 66 h 35 ] LabelGraphics [ text "TAP_BPY" fontSize 13 fontStyle "bold" fontName "Lucida Console"] ]
   node [ id 4 graphics [ type "rectangle" fill "#59FF20" w 50 h 35 ] LabelGraphics [ text "reg" fontSize 13 fontStyle "bold" fontName "Lucida Console"] ]
   edge [ source 0 target 1 label "1" ]
   edge [ source 2147483647 target 2 graphics [ width 1 style "dotted" targetArrow "standard" ] ]
   edge [ source 2 target 3 label "1" ]
   edge [ source 2 target 4 label "2" ]
   edge [ source 2 target 1 graphics [ width 1 style "dashed" targetArrow "standard" ] ]
   edge [ source 0 target 2 label "2" ]
]
