SamacSys ECAD Model
629482/1315552/2.50/2/2/Connector

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "c210_h140"
		(holeDiam 1.4)
		(padShape (layerNumRef 1) (padShapeType Ellipse)  (shapeWidth 2.1) (shapeHeight 2.1))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 2.1) (shapeHeight 2.1))
	)
	(textStyleDef "Default"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 50 mils)
			(strokeWidth 5 mils)
		)
	)
	(patternDef "726386-2" (originalName "726386-2")
		(multiLayer
			(pad (padNum 1) (padStyleRef c210_h140) (pt -2.54, 0) (rotation 90))
			(pad (padNum 2) (padStyleRef c210_h140) (pt 2.54, 0) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt -0.349, 0.075) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt -4 0.4) (pt 4 0.4) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4 0.4) (pt 4 -0.4) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt 4 -0.4) (pt -4 -0.4) (width 0.2))
		)
		(layerContents (layerNumRef 28)
			(line (pt -4 -0.4) (pt -4 0.4) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -4 0.4) (pt -4 -0.4) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 4 0.4) (pt 4 -0.4) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.33 0.4) (pt 1.33 0.4) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.33 -0.4) (pt 1.33 -0.4) (width 0.2))
		)
	)
	(symbolDef "726386-2" (originalName "726386-2")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(line (pt 200 mils 100 mils) (pt 600 mils 100 mils) (width 6 mils))
		(line (pt 600 mils 100 mils) (pt 600 mils -200 mils) (width 6 mils))
		(line (pt 600 mils -200 mils) (pt 200 mils -200 mils) (width 6 mils))
		(line (pt 200 mils -200 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 650 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "726386-2" (originalName "726386-2") (compHeader (numPins 2) (numParts 1) (refDesPrefix J)
		)
		(compPin "1" (pinName "1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "726386-2"))
		(attachedPattern (patternNum 1) (patternName "726386-2")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "Mouser Part Number" "571-726386-2")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/TE-Connectivity-AMP/726386-2?qs=sd1rtLGwt4569WB7PEOdPA%3D%3D")
		(attr "Manufacturer_Name" "TE Connectivity")
		(attr "Manufacturer_Part_Number" "726386-2")
		(attr "Description" "AMP - TE CONNECTIVITY - 726386-2 - TAB, STRAIGHT, 6.3X0.8MM")
		(attr "Datasheet Link" "http://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Customer+Drawing%7F726386%7FG%7Fpdf%7FEnglish%7FENG_CD_726386_G.pdf%7F1-726386-2")
	)

)