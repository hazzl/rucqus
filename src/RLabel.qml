import QtQuick 2.5
import "globals.js" as Global

Text {
    property Item reference

    id: labeltext
    font.pixelSize: Global.smallSize
    color: Global.textColor
    x: reference.x
    anchors.top: reference.bottom
    state: "hidden"
    function hide() {
	if (labeltext.state === "showing") {
	    labeltext.state = "hidden"
	}
    }
    function show() {
	if (labeltext.state === "hidden") {
	    labeltext.state = "showing"
	}
    }
    states: [
	State {
	    name: "hidden"
	    PropertyChanges {
		target: labeltext
		opacity: 0
	    }
	},
	State {
	    name: "showing"
	    PropertyChanges {
		target: labeltext
		opacity: 100
	    }
	}
    ]
    transitions: Transition {
	NumberAnimation {
	    target: labeltext
	    property: "opacity"
	    duration: 500
	    easing.type: Easing.Linear
	}
    }
}
