import QtQuick 2.5
import "globals.js" as Global

Text {
	property Item reference

	font.pixelSize: Global.smallSize
	color: Global.textColor
	x: reference.x
	anchors.top: reference.bottom
}
