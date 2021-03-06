import QtQuick 2.0

// holds vector with a starting point
Item {
    id: root

    // starting point
    property vector3d from: to.minus(vector)

    // end point
    property vector3d to: from.plus(vector)

    // difference end - start
    property vector3d vector: to.minus(from)

    // middle point
    property vector3d middle: from.plus(to).times(0.5)

    // only x and y as vector2d
    property vector2d xypart: Qt.vector2d(vector.x, vector.y)

    // list of points
    property var points: [from, to]

    // vector3d as string
    function vec2str(vec) {
        return '(' + vec.x + ', ' + vec.y + ', ' + vec.z + ')';
    }

    // can direction/length be edited?
    property bool editable: false

    // localized vector as string
    property string text: vec2str(from) + "->" + vec2str(to)

    // TO snapped to which vector?
    property int snappedTo: -1

    // FROM snapped to which vector?
    property int snappedFrom: -1

    // on how many does this depend?
    property int depth: 0

    // how much scale the color?
    property real colorScale: 1. / (0.5 * Math.min(depth - 1, 5) + 1)

    // starting text
    property string fromText: "Start"
    property string toText: "End"

    // starting text when not snapped
    property string fromTextOrig: "Start"
    property string toTextOrig: "End"
}
