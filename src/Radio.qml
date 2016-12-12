import QtQuick 2.5
import "globals.js" as Global

Row {
    anchors.centerIn: parent
    spacing: 5
    Rectangle {
	width: (parent.width - 5) / 2
	height: parent.height
	color: Global.textColor
    }
    ListView {
	id: stationList
	objectName: "stationList"
	signal clicked(int index)
	clip: true
	width: (parent.width - 5) / 2
	preferredHighlightBegin: Global.normalSize * 4
	preferredHighlightEnd: Global.normalSize * 11
	highlightRangeMode: ListView.ApplyRange
	highlightMoveDuration: 600
	highlight: Rectangle {
	    color: "darkgrey"
	    opacity: .2
	}
	delegate: Text {
	    font.pixelSize: ListView.isCurrentItem ? Global.bigSize : Global.normalSize
	    color: Global.textColor
	    text: name
	    MouseArea {
		anchors.fill: parent
		onClicked: {
		    stationList.currentIndex = index
		    stationList.clicked(index)
		}
	    }
	    Behavior on font.pixelSize {
		NumberAnimation {
		    easing.type: Easing.InOutQuart
		    duration: 200
		}
	    }
	}
    }
}
