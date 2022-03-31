/*
  Copyright (C) 2022 RapidSilicon
  Authorized use only
*/

#include "Main/CommandLine.h"
#include "Main/Foedag.h"
#include "Main/ToolContext.h"
#include "MainWindow/Session.h"
#include "MainWindow/main_window.h"
#include "Compiler/CompilerOpenFPGA.h"

namespace RS {

#define Company "RapidSilicon"
#define ToolName "Raptor"
#define ExecutableName "raptor"

QWidget* mainWindowBuilder(FOEDAG::Session* session) {
  FOEDAG::MainWindow* mainW = new FOEDAG::MainWindow{session};
  mainW->MainWindowTitle(std::string(Company) + " " + std::string(ToolName));
  return mainW;
}

void registerAllCommands(QWidget* widget, FOEDAG::Session* session) {
  registerAllFoedagCommands(widget, session);
}

}  // namespace RS

int main(int argc, char** argv) {
  Q_INIT_RESOURCE(main_window_resource);
  FOEDAG::CommandLine* cmd = new FOEDAG::CommandLine(argc, argv);
  cmd->processArgs();

  FOEDAG::GUI_TYPE guiType = FOEDAG::Foedag::getGuiType(cmd->WithQt(), cmd->WithQml());

  FOEDAG::ToolContext* context =
      new FOEDAG::ToolContext(ToolName, Company, ExecutableName);

  FOEDAG::Compiler* compiler = nullptr;
  if (cmd->CompilerName() == "openfpga")
    compiler = new FOEDAG::CompilerOpenFPGA();
  else
    compiler = new FOEDAG::Compiler();
  
  FOEDAG::Foedag* foedag = new FOEDAG::Foedag(cmd, RS::mainWindowBuilder,
                                              RS::registerAllCommands, compiler, context);

  return foedag->init(guiType);
}
