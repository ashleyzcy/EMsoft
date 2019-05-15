/* ============================================================================
 * Copyright (c) 2009-2017 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "PatternControlsWidget.h"

#include <QtCore/QJsonObject>

#include "Common/Constants.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PatternControlsWidget::PatternControlsWidget(QWidget* parent, Qt::WindowFlags windowFlags)
: QWidget(parent, windowFlags)
{
  setupUi(this);

  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PatternControlsWidget::~PatternControlsWidget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternControlsWidget::setupGui()
{
  createValidators();

  createControlsConnections();

  createWidgetConnections();

  opacitySlider->setDisabled(true);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternControlsWidget::on_patternChooserCB_currentIndexChanged(int index)
{
  emit patternChoiceChanged(index);

  PatternControlsWidget::PatternChoice choice = static_cast<PatternControlsWidget::PatternChoice>(index);
  if(choice == PatternControlsWidget::PatternChoice::Composite)
  {
    opacitySlider->setEnabled(true);
  }
  else
  {
    opacitySlider->setDisabled(true);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternControlsWidget::on_rotationStepSize_textChanged(const QString& text)
{
  QString stepSizeStr = text;
  if(stepSizeStr.isEmpty())
  {
    stepSizeStr = "0";
  }

  bool ok = false;
  double rot = text.toDouble(&ok);

  if(ok)
  {
    emit rotationStepSizeChanged(rot);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternControlsWidget::createControlsConnections()
{
  connect(cwBtn, &QPushButton::pressed, [=] { emit controlsChoicePressed(PatternControlsWidget::ControlsChoice::CW); });
  connect(ccwBtn, &QPushButton::pressed, [=] { emit controlsChoicePressed(PatternControlsWidget::ControlsChoice::CCW); });
  connect(upBtn, &QPushButton::pressed, [=] { emit controlsChoicePressed(PatternControlsWidget::ControlsChoice::UP); });
  connect(downBtn, &QPushButton::pressed, [=] { emit controlsChoicePressed(PatternControlsWidget::ControlsChoice::DOWN); });
  connect(leftBtn, &QPushButton::pressed, [=] { emit controlsChoicePressed(PatternControlsWidget::ControlsChoice::LEFT); });
  connect(rightBtn, &QPushButton::pressed, [=] { emit controlsChoicePressed(PatternControlsWidget::ControlsChoice::RIGHT); });
  connect(zeroBtn, &QPushButton::pressed, [=] { emit controlsChoicePressed(PatternControlsWidget::ControlsChoice::ZERO); });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternControlsWidget::createValidators()
{
  QDoubleValidator* doubleValidator = new QDoubleValidator(rotationStepSize);
  rotationStepSize->setValidator(doubleValidator);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternControlsWidget::createWidgetConnections()
{
  connect(opacitySlider, &QSlider::valueChanged, this, [=](int value) {
    double opacity = static_cast<double>(value) / 100;
    emit opacityChanged(opacity);
  });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternControlsWidget::readSession(QJsonObject& obj)
{
  patternChooserCB->setCurrentText(obj[EMsoftWorkbenchConstants::IOStrings::PatternSelection].toString());
  opacitySlider->setValue(obj[EMsoftWorkbenchConstants::IOStrings::Opacity].toInt());
  rotationStepSize->setText(QString::number(obj[EMsoftWorkbenchConstants::IOStrings::RotationStepSize].toDouble()));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternControlsWidget::writeSession(QJsonObject& obj)
{
  obj[EMsoftWorkbenchConstants::IOStrings::PatternSelection] = patternChooserCB->currentText();
  obj[EMsoftWorkbenchConstants::IOStrings::Opacity] = opacitySlider->value();
  obj[EMsoftWorkbenchConstants::IOStrings::RotationStepSize] = rotationStepSize->text().toDouble();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double PatternControlsWidget::getRotationStepSize()
{
  return rotationStepSize->text().toDouble();
}
