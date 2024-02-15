/*
  Copyright (C) 2022 RapidSilicon
  Authorized use only
*/

#include "Compiler/CompilerRS.h"
#include "MPW1Loader.h"
#include "Main/CommandLine.h"
#include "Main/Foedag.h"
#include "Main/Settings.h"
#include "Main/ToolContext.h"
#include "Main/WidgetFactory.h"
#include "MainWindow/Session.h"
#include "MainWindow/main_window.h"
#include "PinAssignment/PinAssignmentCreator.h"
#include "Simulation/Simulator.h"

namespace RS {

#define Company "Rapid Silicon"
#define ToolName "Raptor Design Suite"
#define ExecutableName "raptor"

QWidget* mainWindowBuilder(FOEDAG::Session* session) {
  FOEDAG::MainWindow* mainW = new FOEDAG::MainWindow{session};
  auto info = mainW->Info();
  info.name = QString("%1 %2").arg(Company, ToolName);
  info.url.clear();
  std::filesystem::path p{".."};
  p = p / "licenses" / "rs-eula.txt";
  info.licenseFile = QString::fromStdString(p.string());
  mainW->Info(info);
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
    compiler->SetParserType(FOEDAG::Compiler::ParserType::Default);
    FOEDAG::addTclArgFns("CompilerRs_Synthesis",
                         {FOEDAG::TclArgs_setRsSynthesisOptions,
                          FOEDAG::TclArgs_getRsSynthesisOptions});
  }

  FOEDAG::Foedag* foedag =
      new FOEDAG::Foedag(cmd, RS::mainWindowBuilder, RS::registerAllCommands,
                         compiler, settings, context);
  FOEDAG::PinAssignmentCreator::RegisterPackagePinLoader(
      "MPW1", new FOEDAG::MPW1Loader{nullptr});
  if (opcompiler) {
    std::filesystem::path binpath = foedag->Context()->BinaryPath();
    context->ProgrammerGuiPath(binpath / "programmer_gui");
    std::filesystem::path datapath = foedag->Context()->DataPath();
    std::filesystem::path analyzePath = binpath / "analyze";
    std::filesystem::path yosysPath = binpath / "yosys";
    std::filesystem::path vprPath = binpath / "vpr";
    std::filesystem::path finalizePath = binpath / "finalize";
    std::filesystem::path openFpgaPath = binpath / "openfpga";
    std::filesystem::path pinConvPath = binpath / "pin_c";
    std::filesystem::path staPath = binpath / "sta";
    std::filesystem::path starsPath = binpath / "stars";
    std::filesystem::path tclPath = binpath / "tclsh8.6";
    std::filesystem::path raptorPath = binpath / ExecutableName;
    std::filesystem::path bitstreamSettingPath =
        datapath / "etc" / "devices" / "gemini" / "bitstream_annotation.xml";
    std::filesystem::path simSettingPath =
        datapath / "etc" / "devices" / "gemini" / "fixed_sim_openfpga.xml";
    std::filesystem::path repackConstraintPath = datapath / "etc" / "devices" /
                                                 "gemini" /
                                                 "repack_design_constraint.xml";
    std::filesystem::path openOcdPath = binpath / "openocd";
    opcompiler->AnalyzeExecPath(analyzePath);
    opcompiler->YosysExecPath(yosysPath);
    opcompiler->VprExecPath(vprPath);
    opcompiler->ReConstructVExecPath(finalizePath);
    opcompiler->StaExecPath(staPath);
    opcompiler->StarsExecPath(starsPath);
    opcompiler->TclExecPath(tclPath);
    opcompiler->RaptorExecPath(raptorPath);
    opcompiler->OpenFpgaExecPath(openFpgaPath);
    opcompiler->OpenFpgaBitstreamSettingFile(bitstreamSettingPath);
    opcompiler->OpenFpgaSimSettingFile(simSettingPath);
    opcompiler->OpenFpgaRepackConstraintsFile(repackConstraintPath);
    opcompiler->PinConvExecPath(pinConvPath);
    opcompiler->ProgrammerToolExecPath(openOcdPath);
    opcompiler->GetSimulator()->SetSimulatorPath(
        FOEDAG::Simulator::SimulatorType::Verilator,
        (binpath / "HDL_simulator" / "verilator" / "bin").string());
    opcompiler->GetSimulator()->SetSimulatorPath(
        FOEDAG::Simulator::SimulatorType::GHDL,
        (binpath / "HDL_simulator" / "GHDL" / "bin").string());
    opcompiler->GetSimulator()->SetSimulatorPath(
        FOEDAG::Simulator::SimulatorType::Icarus,
        (binpath / "HDL_simulator" / "iverilog" / "bin").string());
    std::filesystem::path configFileSearchDir = datapath / "configuration";
    opcompiler->SetConfigFileSearchDirectory(configFileSearchDir);
  }
  return foedag->init(guiType);
}
