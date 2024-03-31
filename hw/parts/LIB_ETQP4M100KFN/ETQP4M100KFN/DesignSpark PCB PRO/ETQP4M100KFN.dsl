SamacSys ECAD Model
2242255/1315552/2.50/2/2/Inductor

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r270_220"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 2.2) (shapeHeight 2.7))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Default"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 50 mils)
			(strokeWidth 5 mils)
		)
	)
	(patternDef "ETQ-P4M_KFN" (originalName "ETQ-P4M_KFN")
		(multiLayer
			(pad (padNum 1) (padStyleRef r270_220) (pt -2.85, 0) (rotation 90))
			(pad (padNum 2) (padStyleRef r270_220) (pt 2.85, 0) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt -3 3) (pt 3 3) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt 3 3) (pt 3 -3) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt 3 -3) (pt -3 -3) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt -3 -3) (pt -3 3) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3 3) (pt 3 3) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 3 -3) (pt -3 -3) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -5.2, 0) (radius 0.05) (startAngle 0.0) (sweepAngle 0.0) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -5.2, 0) (radius 0.05) (startAngle 180.0) (sweepAngle 180.0) (width 0.2))
		)
		(layerContents (layerNumRef 30)
			(line (pt -6.1 4) (pt 5 4) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 5 4) (pt 5 -4) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 5 -4) (pt -6.1 -4) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt -6.1 -4) (pt -6.1 4) (width 0.05))
		)
	)
	(symbolDef "ETQP4M100KFN" (originalName "ETQP4M100KFN")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 0 mils -35 mils) (rotation 0]) (justify "UpperLeft") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 800 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 800 mils -35 mils) (rotation 0]) (justify "UpperRight") (textStyleRef "Default"))
		))
		(arc (pt 250 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 350 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 450 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 550 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 650 mils 250 mils) (justify Left) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "ETQP4M100KFN" (originalName "ETQP4M100KFN") (compHeader (numPins 2) (numParts 1) (refDesPrefix L)
		)
		(compPin "1" (pinName "1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "ETQP4M100KFN"))
		(attachedPattern (patternNum 1) (patternName "ETQ-P4M_KFN")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "TME Electronic Components Part Number" "")
		(attr "TME Electronic Components Price/Stock" "")
		(attr "Manufacturer_Name" "Panasonic")
		(attr "Manufacturer_Part_Number" "ETQP4M100KFN")
		(attr "Description" "Power Choke Coil, Automotive Grade, 10uF")
		(attr "Datasheet Link" "http://industrial.panasonic.com/cdbs/www-data/pdf/AGL0000/AGL0000C73.pdf")
		(attr "Height" "4.8 mm")
	)

)
