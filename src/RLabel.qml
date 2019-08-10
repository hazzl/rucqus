import QtQuick 2.12
import "globals.js" as Global

Text {
    property Item reference

    id: labeltext
    font.pixelSize: Global.smallSize
    font.family: Global.fontFamily
    color: Global.textColor
    x: reference.x
    anchors.top: reference.bottom
    opacity: 0.0
    Behavior on opacity {
	    OpacityAnimator{
		    duration: 500
	    }
    }
}
