import QtQuick 2.5
import QtQuick.Controls 2.0
import "globals.js" as Global

Item
{
    Item {
	width: parent.width - 16
	anchors.horizontalCenter: parent.horizontalCenter
	IButton {
	    id: genreButton
	    text: "Genre"
	    anchors.top: parent.top
	    anchors.topMargin: 0
	    anchors.left: parent.left
	    anchors.leftMargin: 0
	    iconSource: "../icons/Genre.png"
	    onClicked: genreLView.toggle()
	}
	RLabel {
	    id: genreLabel
	    reference: genreButton
	}
	IButton {
	    id: artistButton
	    text: "Artist"
	    anchors.horizontalCenter: parent.horizontalCenter
	    anchors.top: parent.top
	    iconSource: "../icons/Artist.png"
	    onClicked: artistLView.toggle()
	}
	RLabel {
	    id: artistLabel
	    reference: artistButton
	}
	IButton {
	    id: albumButton
	    text: "Album"
	    anchors.top: parent.top
	    anchors.right: parent.right
	    iconSource: "../icons/Album.png"
	    onClicked: albumLView.toggle()
	}
	RLabel {
	    id: albumLabel
	    reference: albumButton
	}
	SQListView {
	    id: genreLView
	    objectName: "genreLView"
	    reference: genreLabel
	    model: genreModel
	    Component.onCompleted: {
		artistLView.showing.connect(hide)
		albumLView.showing.connect(hide)
		playbutton.clicked.connect(hide)
	    }
	}
	SQListView {
	    id: artistLView
	    objectName: "artistLView"
	    reference: artistLabel
	    model: artistModel
	    Component.onCompleted: {
		genreLView.showing.connect(hide)
		albumLView.showing.connect(hide)
		playbutton.clicked.connect(hide)
		genreLView.currentIndexChanged.connect(clearCurrent)
	    }
	}
	SQListView {
	    id: albumLView
	    objectName: "albumLView"
	    model: albumModel
	    reference: artistLabel
	    section.property: "category"
	    section.criteria: ViewSection.FullString
	    section.delegate: Text {
		text: section
		font.pixelSize: Global.normalSize
		font.italic: true
		font.family: Global.fontFamily
		color: "white"
		height: Global.bigSize
		verticalAlignment: Text.AlignBottom
		Rectangle {
		    color: Global.textColor
		    visible: section != ''
		    height: 2
		    width: 192
		    anchors.top: parent.baseline
		}
	    }
	    Component.onCompleted: {
		artistLView.showing.connect(hide)
		genreLView.showing.connect(hide)
		playbutton.clicked.connect(hide)
		genreLView.currentIndexChanged.connect(clearCurrent)
		artistLView.currentIndexChanged.connect(clearCurrent)
	    }
	}
	Image {
	    id: cover
	    objectName: "cover"
	    x: parent.width / 4 - width / 2
	    height: 230
	    width: 230
	    anchors.top: genreLabel.bottom
	    anchors.topMargin: 10 + (viewPort.height - 30 - height - genreButton.height - genreLabel.height - playbutton.height)/2
	    opacity: 1
	    fillMode: Image.PreserveAspectFit
	    states: State {
		name: "hidden"
		when: (genreLView.state === 'showing')
		      || (artistLView.state === 'showing')
		      || (albumLView.state === 'showing')
		PropertyChanges {
		    target: cover
		    x: -width
		    opacity: 0
		}
	    }
	    transitions: Transition {
		NumberAnimation {
		    target: cover
		    properties: "x,opacity"
		    duration: 350
		    easing.type: Easing.InOutQuad
		}
	    }
	}
	Connections {
	    target: song
	    onMetaDataChanged: {
		switch (key) {
		case "Genre":
		    genreLabel.text = value
		    break
		case "AlbumTitle":
		    albumLabel.text = value
		    break
		case "ContributingArtist":
		    artistLabel.text = value
		    break
		case "CoverArtUrlLarge":
		    cover.source = value
		}
	    }
	    onSourceChanged: {
		var url = song.source.toString()
		cover.source = url.substring(0, url.lastIndexOf(
						 '/') + 1) + "album.jpg"
	    }
	    onStateChanged: {
		switch (song.state) {
		case Global.PausedState:

		case Global.StoppedState:
		    genreLabel.opacity = 0.0
		    artistLabel.opacity = 0.0
		    albumLabel.opacity = 0.0
		    break
		case Global.PlayingState:
		    genreLabel.opacity = 1.0
		    artistLabel.opacity = 1.0
		    albumLabel.opacity = 1.0
		}
	    }
	}
	ListView {
	    id: playList
	    objectName: "pListView"
	    signal clicked(int index)

	    Connections {
		target: song
		onCurrentIndexChanged: if (playList.currentIndex != index)
					   playList.currentIndex = index
	    }
	    Connections {
		target: plistModel
		onNewData: playList.currentIndex = 0
	    }

	    model: plistModel
	    anchors.top: genreLabel.bottom
	    anchors.topMargin: 10
	    height: playbutton.y - y - 3
	    anchors.right: parent.right
	    width: parent.width / 2
	    clip: true
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
		font.family: Global.fontFamily
		color: Global.textColor
		text: name
		MouseArea {
		    anchors.fill: parent
		    onClicked: {
			playList.currentIndex = index
			playList.clicked(index)
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
    IButton {
	id: playbutton
	objectName: "playbutton"
	anchors.bottom: parent.bottom
	anchors.left: parent.left
	anchors.margins: 8
	width: 58
	iconSource: song.state === Global.PlayingState ? "../icons/Pause.png" : "../icons/Play.png"
	onClicked: song.state === Global.PlayingState ? song.pause() : song.play()
    }
    Slider {
	id: slider
	objectName: "slider"
	width: parent.width - 212
	height: Global.normalSize
	anchors.verticalCenter: playbutton.verticalCenter
	anchors.left: playbutton.right
	anchors.leftMargin: 8
	value: song.position / song.duration
	onPressedChanged: if (!pressed)
			      song.position = (song.duration * value)
    }
    Text {
	id: timer
	anchors.verticalCenter: slider.verticalCenter
	anchors.right: parent.right
	anchors.rightMargin: 8
	text: sec2min(Math.round(
			  slider.value * song.duration / 1000)) + "/" + sec2min(
		  Math.round(song.duration / 1000))
	font.pixelSize: Global.normalSize
	font.family: Global.fontFamily
	color: Global.textColor
	function sec2min(sec) {
	    var ret = ""
	    if (sec > 3599) {
		ret = Math.floor(sec / 3600).toString() + ":"
		sec %= 3600
	    }
	    var mins = Math.floor(sec / 60)
	    if ((ret.length > 0) && (mins < 10))
		ret += "0"
	    ret += mins.toString() + ":"
	    sec %= 60
	    if (sec < 10)
		ret += "0"
	    return ret + sec.toString()
	}
    }
}
