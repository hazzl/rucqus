import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
    Grid {
        columns: 2
        spacing: 32
        anchors.centerIn: parent
        SLabel {
            text: "Database"
        }
        TextField {
            text: "/home/mozart/rucqus.sqlite"
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
            property int factor: Math.pow(10,decimals)
            id: spinbox
            from: 50
            value: 100
            to: 200
            stepSize: 1
            Component.onCompleted: { value = config.scaleFactor * factor }

            onValueChanged: {
                config.scaleFactor = value / factor
            }

            validator: DoubleValidator {
                bottom: Math.min(spinbox.from, spinbox.to)
                top: Math.max(spinbox.from, spinbox.to)
            }

            textFromValue: function(value, locale) {
                return parseFloat(value*1.0/factor).toFixed(decimals);
            }
        }
    }
}
