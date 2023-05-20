library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
-- FPGA projects using Verilog code VHDL code
-- fpga4student.com: FPGA projects, Verilog projects, VHDL projects
-- VHDL project: VHDL code for counters with testbench  
-- VHDL project: VHDL code for up counter   
entity UP_COUNTER is
    Port ( clock0: in std_logic; -- clock input
           reset: in std_logic; -- reset input 
           counter: out std_logic_vector(3 downto 0) -- output 4-bit counter
     );
end UP_COUNTER;

architecture Behavioral of UP_COUNTER is
signal counter_up: std_logic_vector(3 downto 0);
begin
-- up counter
process(clock0)
begin
if(rising_edge(clock0)) then
    if(reset='1') then
         counter_up <= x"0";
    else
        counter_up <= counter_up + x"1";
    end if;
 end if;
end process;
 counter <= counter_up;

end Behavioral;
