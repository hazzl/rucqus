import QtQuick 2.12
import QtQuick.Controls 2.5
import "globals.js" as Global

Item {
    Grid {
        columns: 2
        spacing: 32
        anchors.centerIn: parent
        SLabel {
            text: "Library Path"
        }

        Row {
            spacing: 10
            TextField {
                text: config.libraryPath
                placeholderText: "Enter Library Path"
                onEditingFinished: config.libraryPath = text
            }
            AnimatedImage {
                height: 32
                source: reader.state === Global.PlayingState ?
                            "qrc:/icons/scanning.gif"
                          :
                            reader.state === Global.StoppedState ?
                                "qrc:/icons/check.png"
                              :
                                "qrc:/icons/error.png"
                onStatusChanged: playing = (status == AnimatedImage.Ready)
                MouseArea {
                    objectName: "rescanButton"
                    anchors.fill: parent
                    onClicked: reader.scanLibrary()
                }
            }
        }

        SLabel {
            text: "Hide Pointer"
        }
        CheckBox {
            checked: config.hidePointer
            onCheckStateChanged: config.hidePointer = checkState
        }
        SLabel {
            text: "Vert. Stretch"
        }
        SpinBox {
            property int decimals: 2
            property int factor: Math.pow(10, decimals)
            id: spinbox
            from: 50
            value: 100
            to: 200
            stepSize: 1
            Component.onCompleted: {
                value = config.scaleFactor * factor
            }

            onValueChanged: {
                config.scaleFactor = value / factor
            }

            validator: DoubleValidator {
                bottom: Math.min(spinbox.from, spinbox.to)
                top: Math.max(spinbox.from, spinbox.to)
            }

            textFromValue: function (value, locale) {
                return parseFloat(value * 1.0 / factor).toFixed(decimals)
            }
        }
    }
}
