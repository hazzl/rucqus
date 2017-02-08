import QtQuick 2.5
import "globals.js" as Global

Item {
	Row {
		width: parent.width - 16
		height: parent.height - 24
		anchors.centerIn: parent
		spacing: 10
		Column {
			id: column
			width: parent.width/2
			height: parent.height

			Item {
				width: parent.width
				height: parent.height/2

				Image {
					id: stationImage
					anchors.centerIn: parent
					visible: false
				}
			}

			Text {
				id: metadataText
			}
		}

		ListView {
			id: stationList
			objectName: "stationListV"
			signal clicked(int index)

			clip: true
			width: (parent.width - 5) / 2
			height: parent.height
			model: radioModel
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
}
