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

				Image {
					id: stationImage
					width: parent.width
					height: parent.height/2
					fillMode: Image.PreserveAspectFit
					smooth: true
					source: "file:///home/hazzl/Schreibtisch/"+stationList.currentItem.delText+".png"
				}
			Text {
				id: metadataText
				color: Global.textColor
				font.pixelSize: Global.normalSize
				width: parent.width
				wrapMode: Text.Wrap
				clip: true
				Connections {
					target: song
					onMetaDataChanged: {
						console.log(key+":",value)
						if (key === "Title") { metadataText.text = value }
					}
				}
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
			preferredHighlightBegin: Global.bigSize * 4
			preferredHighlightEnd: Global.bigSize * 11
			highlightRangeMode: ListView.ApplyRange
			highlightMoveDuration: 600
			highlight: Rectangle {
				color: "darkgrey"
				opacity: .2
			}
			delegate: Text {
				property string delText: text
				font.pixelSize: Global.bigSize
				color: Global.textColor
				text: name
				MouseArea {
					anchors.fill: parent
					onClicked: {
						stationList.currentIndex = index
						stationList.clicked(index)
						metadataText.text = ""
					}
				}
			}
		}
	}
}
