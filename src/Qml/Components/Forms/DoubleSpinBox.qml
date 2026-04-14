import QtQuick
import QtQuick.Controls

SpinBox {
    id: root
    property int decimals: 2

    from: 0
    to: 999999900
    stepSize: 100 // 10^decimals
    editable: true
    wheelEnabled: true

    validator: DoubleValidator {
        bottom: 0
        top: 99999999
        decimals: root.decimals
        notation: DoubleValidator.StandardNotation
        locale: "ru_RU"
    }

    textFromValue: function(value, locale) {
        return Number(value / 100).toLocaleString(locale, 'f', root.decimals)
    }

    valueFromText: function(text, locale) {
        return Math.round(Number.fromLocaleString(locale, text) * 100)
    }
}
