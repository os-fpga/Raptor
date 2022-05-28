/*
  Copyright (C) 2022 RapidSilicon
  Authorized use only
*/

#include "Compiler/CompilerRS.h"
#include "Main/CommandLine.h"
#include "Main/Foedag.h"
#include "Main/Settings.h"
#include "Main/ToolContext.h"
#include "MainWindow/Session.h"
#include "MainWindow/main_window.h"

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

  FOEDAG::GUI_TYPE guiType =
      FOEDAG::Foedag::getGuiType(cmd->WithQt(), cmd->WithQml());

  FOEDAG::ToolContext* context =
      new FOEDAG::ToolContext(ToolName, Company, ExecutableName);

  FOEDAG::Settings* settings = new FOEDAG::Settings();

  FOEDAG::Compiler* compiler = nullptr;
  FOEDAG::CompilerRS* opcompiler = nullptr;
  if (cmd->CompilerName() == "dummy") {
    compiler = new FOEDAG::Compiler();
  } else {
    opcompiler = new FOEDAG::CompilerRS();
    compiler = opcompiler;
    compiler->SetUseVerific(true);
  }

  FOEDAG::Foedag* foedag =
      new FOEDAG::Foedag(cmd, RS::mainWindowBuilder, RS::registerAllCommands,
                         compiler, settings, context);

  if (opcompiler) {
    std::filesystem::path binpath = foedag->Context()->BinaryPath();
    std::filesystem::path datapath = foedag->Context()->DataPath();
    std::filesystem::path yosysPath = binpath / "yosys";
    std::filesystem::path vprPath = binpath / "vpr";
    std::filesystem::path openFpgaPath = binpath / "openfpga";
    std::filesystem::path pinConvPath = binpath / "pin_c";
    std::filesystem::path archPath =
        datapath / "etc" / "devices" / "gemini" / "gemini_vpr.xml";
    std::filesystem::path openFpgaArchPath =
        datapath / "etc" / "devices" / "gemini" / "gemini_openfpga.xml";
    std::filesystem::path bitstreamSettingPath =
        datapath / "etc" / "devices" / "gemini" / "bitstream_annotation.xml";
    std::filesystem::path simSettingPath =
        datapath / "etc" / "devices" / "gemini" / "fixed_sim_openfpga.xml";
    std::filesystem::path repackConstraintPath = datapath / "etc" / "devices" /
                                                 "gemini" /
                                                 "repack_design_constraint.xml";

    opcompiler->YosysExecPath(yosysPath);
    opcompiler->VprExecPath(vprPath);
    opcompiler->OpenFpgaExecPath(openFpgaPath);
    opcompiler->ArchitectureFile(archPath);
    opcompiler->OpenFpgaArchitectureFile(openFpgaArchPath);
    opcompiler->OpenFpgaBitstreamSettingFile(bitstreamSettingPath);
    opcompiler->OpenFpgaSimSettingFile(simSettingPath);
    opcompiler->OpenFpgaRepackConstraintsFile(repackConstraintPath);
    opcompiler->PinConvExecPath(pinConvPath);
  }
  return foedag->init(guiType);
}
