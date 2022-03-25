/*
  Copyright (C) 2022 RapidSilicon
  Authorized use only
*/

#include "Main/CommandLine.h"
#include "Main/Foedag.h"
#include "Main/ToolContext.h"
#include "MainWindow/Session.h"
#include "MainWindow/main_window.h"

namespace RS {

#define Company "RapidSilicon"
#define ToolName "Raptor"
#define ExecutableName "raptor"

QWidget* mainWindowBuilder(FOEDAG::CommandLine* cmd,
                           FOEDAG::TclInterpreter* interp) {
  FOEDAG::MainWindow* mainW = new FOEDAG::MainWindow{interp};
  mainW->MainWindowTitle(std::string(Company) + " " + std::string(ToolName));
  return mainW;
}

void registerAllCommands(QWidget* widget, FOEDAG::Session* session) {
  registerAllFoedagCommands(widget, session);
}

}  // namespace RS

FOEDAG::GUI_TYPE getGuiType(const bool& withQt, const bool& withQml) {
  if (!withQt) return FOEDAG::GUI_TYPE::GT_NONE;
  if (withQml)
    return FOEDAG::GUI_TYPE::GT_QML;
  else
    return FOEDAG::GUI_TYPE::GT_WIDGET;
}

int main(int argc, char** argv) {
  Q_INIT_RESOURCE(main_window_resource);
  FOEDAG::CommandLine* cmd = new FOEDAG::CommandLine(argc, argv);
  cmd->processArgs();

  FOEDAG::GUI_TYPE guiType = getGuiType(cmd->WithQt(), cmd->WithQml());
  FOEDAG::ToolContext* context =
      new FOEDAG::ToolContext(ToolName, Company, ExecutableName);
  FOEDAG::Foedag* foedag = new FOEDAG::Foedag(cmd, RS::mainWindowBuilder,
                                              RS::registerAllCommands, context);

  return foedag->init(guiType);
}
