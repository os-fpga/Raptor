/*
Copyright 2022 The Foedag team

GPL License

Copyright (c) 2022 The Open-Source FPGA Foundation

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>

#include "Compiler/CompilerRS.h"
#include "Compiler/TaskManager.h"
#include "Configuration/CFGCompiler/CFGCompiler.h"
#include "Main/Foedag.h"
#include "ProgrammerGui/ProgrammerMain.h"

#define Company "Rapid Silicon"
#define ToolName "Rapid Programmer"
#define ExecutableName "raptor"

QWidget* mainWindowBuilder(FOEDAG::Session* session) {
  return new FOEDAG::ProgrammerMain{};
}

void registerAllCommands(QWidget* widget, FOEDAG::Session* session) {
  auto compiler = session->GetCompiler();
  auto cfgCompiler = new FOEDAG::CFGCompiler(compiler);
  compiler->SetConfiguration(cfgCompiler);
  cfgCompiler->RegisterCommands(session->TclInterp(), false);
}

int main(int argc, char* argv[]) {
  FOEDAG::CommandLine* cmd = new FOEDAG::CommandLine(argc, argv);
  cmd->processArgs();

  FOEDAG::GUI_TYPE guiType =
      FOEDAG::Foedag::getGuiType(cmd->WithQt(), cmd->WithQml());

  FOEDAG::ToolContext* context =
      new FOEDAG::ToolContext(ToolName, Company, ExecutableName);

  FOEDAG::Compiler* compiler = new FOEDAG::CompilerRS;
  auto taskM = new FOEDAG::TaskManager(compiler);
  compiler->setTaskManager(taskM);
  FOEDAG::Settings* settings = new FOEDAG::Settings();
  FOEDAG::Foedag* foedag = new FOEDAG::Foedag(
      cmd, mainWindowBuilder, registerAllCommands, compiler, settings, context);
  std::filesystem::path binpath = foedag->Context()->BinaryPath();
  std::filesystem::path openOcdPath = binpath / "openocd";
  std::filesystem::path datapath = foedag->Context()->DataPath();
  compiler->ProgrammerToolExecPath(openOcdPath);
  return foedag->init(guiType);
}
