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
	    id: spinbox
	    from: 50
	    value: config.scaleFactor * 100
	    to: 200
	    stepSize: 1

	    property int decimals: 2

	    onValueChanged: {
		    config.scaleFactor = value / 100
	    }

	    validator: DoubleValidator {
		bottom: Math.min(spinbox.from, spinbox.to)
		top: Math.max(spinbox.from, spinbox.to)
	    }

	    textFromValue: function (value, locale) {
		return Number(value / 100).toLocaleString(locale, 'f',
							  spinbox.decimals)
	    }

	    valueFromText: function (text, locale) {
		return Number.fromLocaleString(locale, text) * 100
	    }
	}
    }
}
