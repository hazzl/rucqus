import QtQuick 2.5

Item {
    id: chooser
    signal activated(string value)

    Row {
	id: row1
	anchors.centerIn: parent
	spacing: 32

	IButton {
	    id: musicButton
	    text: "Music"
	    iconSource: "../icons/Genre.png"
	    onClicked: chooser.activated("MusicPlayer.qml")
	}

	IButton {
	    id: radioButton
	    text: "Radio"
	    iconSource: "../icons/Radio.png"
	}

	IButton {
	    id: prefsButton
	    text: "Preferences"
	    width: 192
	    iconSource: "../icons/Prefs.png"
	    onClicked: chooser.activated("Prefs.qml")
	}
    }
}
