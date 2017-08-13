import QtQuick.Window 2.0
import QtQuick 2.0
import QtQuick.Controls 2.0

Window {
    visible: true
    width: 640
    height: 480
    Rectangle {
	id: viewPort
	visible: true
	width: 640
	height: 480 / config.scaleFactor
	color: "black"
	transform: Scale {
	    yScale: config.scaleFactor
	}
	SwipeView {
	    id: sview
	    anchors.fill: parent
	    currentIndex: 1
	    Item {
		id: firstPage
		Chooser {
		    anchors.fill: parent
		    onActivated: {
			secondPage.setSource(value)
			sview.currentIndex = 1
		    }
		}
	    }
	    Loader {
		id: secondPage
		objectName: "loaderPage"
	    }
	}
    }
}
