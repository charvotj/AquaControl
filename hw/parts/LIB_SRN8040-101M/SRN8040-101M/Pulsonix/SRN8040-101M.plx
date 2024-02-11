PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//849615/1315552/2.50/2/4/Inductor

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r820_260"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 2.600) (shapeHeight 8.200))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Normal"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 1.27)
			(strokeWidth 0.127)
		)
	)
	(patternDef "SRN80408R2Y" (originalName "SRN80408R2Y")
		(multiLayer
			(pad (padNum 1) (padStyleRef r820_260) (pt -2.800, 0.000) (rotation 0))
			(pad (padNum 2) (padStyleRef r820_260) (pt 2.800, 0.000) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt -0.400, 0.000) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt -4 4) (pt 4 4) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4 4) (pt 4 -4) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4 -4) (pt -4 -4) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -4 -4) (pt -4 4) (width 0.025))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -5.9 5.1) (pt 5.1 5.1) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 5.1 5.1) (pt 5.1 -5.1) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 5.1 -5.1) (pt -5.9 -5.1) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -5.9 -5.1) (pt -5.9 5.1) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1 4) (pt 1 4) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1 -4) (pt 1 -4) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -4.8 0.1) (pt -4.8 0.1) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -4.8, 0) (radius 0.1) (startAngle 90.0) (sweepAngle -180.0) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -4.8 -0.1) (pt -4.8 -0.1) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -4.8, 0) (radius 0.1) (startAngle 270) (sweepAngle -180.0) (width 0.2))
		)
	)
	(symbolDef "SRN8040-101M" (originalName "SRN8040-101M")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 0 mils -35 mils) (rotation 0]) (justify "UpperLeft") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 800 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 800 mils -35 mils) (rotation 0]) (justify "UpperRight") (textStyleRef "Normal"))
		))
		(arc (pt 250 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 350 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 450 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 550 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 650 mils 250 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 650 mils 150 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "SRN8040-101M" (originalName "SRN8040-101M") (compHeader (numPins 2) (numParts 1) (refDesPrefix L)
		)
		(compPin "1" (pinName "1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "SRN8040-101M"))
		(attachedPattern (patternNum 1) (patternName "SRN80408R2Y")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "Mouser Part Number" "652-SRN8040-101M")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Bourns/SRN8040-101M?qs=opBjA1TV902pEUqOZBsqAg%3D%3D")
		(attr "Manufacturer_Name" "Bourns")
		(attr "Manufacturer_Part_Number" "SRN8040-101M")
		(attr "Description" "Bourns SRN8040 Series Type 8040 Shielded Wire-wound SMD Inductor with a Ferrite Core, 100 uH +/-20% Wire-Wound 1A Idc")
		(attr "<Hyperlink>" "https://www.bourns.com/pdfs/SRN8040.pdf")
		(attr "<Component Height>" "4")
		(attr "<STEP Filename>" "SRN8040-101M.stp")
		(attr "<STEP Offsets>" "X=-0.03;Y=0;Z=3.95")
		(attr "<STEP Rotation>" "X=0;Y=0;Z=-90")
	)

)
