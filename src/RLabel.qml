import QtQuick 2.5
import "globals.js" as Global

Text {
    property Item reference

    id: labeltext
    font.pixelSize: Global.smallSize
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
