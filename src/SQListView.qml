import QtQuick 2.12
import "globals.js" as Global

ListView {
    id: sqlistView
    signal showing
    property Item reference

    state: "hidden"
    clip: true
    width: (parent.width - 16) / 2
    highlight: Rectangle {color: "darkgrey"; radius: 2; opacity:.3}
    highlightMoveDuration: 600
    currentIndex: -1

    function toggle() {
        if (sqlistView.state === "hidden")
            sqlistView.state = "showing"
        else
            sqlistView.state = "hidden"
    }

    function hide() {
	if (sqlistView.state === "showing") { sqlistView.state = "hidden"}
    }
    function clearCurrent() {
	    currentIndex = -1
    }

    delegate: Text {
            id: delegateText
            text: name
            color: Global.textColor
            font.pixelSize: Global.bigSize
            font.family: Global.fontFamily
            height: Global.bigSize+4
            MouseArea {
                anchors.fill: parent
                onClicked: {
			if (sqlistView.currentIndex == index)
				sqlistView.currentIndex = -1
			else
				sqlistView.currentIndex=index
		}
            }
	}

    states: [
	State {
	    name: "hidden"
	    PropertyChanges {
		target: sqlistView
		x: reference.x
		y: reference.y 
		height: reference.height
		opacity: 0
		enabled: false
	    }
	}, State {
	    name: "showing"
	    PropertyChanges {
		target: sqlistView
		x: genreLabel.x
		y: genreLabel.y + genreLabel.height
		height: playbutton.y - y - 3
		opacity: 1
		enabled: true
	    }
	    StateChangeScript { script: sqlistView.showing() }
	}
    ]
    transitions: Transition {
	NumberAnimation {
	    target: sqlistView
	    properties: "x,y,opacity,height"
	    duration: 300
	    easing.type: Easing.InOutQuad
	}
    }
}
