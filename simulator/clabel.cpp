/***************************************************************
 *
 *                 This code is part of LUA_EOS
 *
 * Copyrights 2021 - Varanda Labs Inc.
 *
 * License:
 *   Creative Commons: CC BY-NC-SA 4.0
 *   Attribution-NonCommercial-ShareAlike 4.0 International
 *   https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
 *
 *   Note: for purchasing a commercial license contact:
 *     m@varanda.ca
 *
 ***************************************************************
 */

#include "clabel.h"
#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    mousePressed(event->x(), event->y());
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event) {
    mouseReleased(event->x(), event->y());
}

