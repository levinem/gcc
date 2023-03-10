------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--                 A D A . T A S K _ T E R M I N A T I O N                  --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--            Copyright (C) 2005, Free Software Foundation, Inc.            --
--                                                                          --
-- This specification is derived from the Ada Reference Manual for use with --
-- GNAT. The copyright notice above, and the license provisions that follow --
-- apply solely to the  contents of the part following the private keyword. --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 2,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License --
-- for  more details.  You should have  received  a copy of the GNU General --
-- Public License  distributed with GNAT;  see file COPYING.  If not, write --
-- to  the  Free Software Foundation,  51  Franklin  Street,  Fifth  Floor, --
-- Boston, MA 02110-1301, USA.                                              --
--                                                                          --
-- As a special exception,  if other files  instantiate  generics from this --
-- unit, or you link  this unit with other files  to produce an executable, --
-- this  unit  does not  by itself cause  the resulting  executable  to  be --
-- covered  by the  GNU  General  Public  License.  This exception does not --
-- however invalidate  any other reasons why  the executable file  might be --
-- covered by the  GNU Public License.                                      --
--                                                                          --
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

with System.Tasking;
--  used for Task_Id

with System.Task_Primitives.Operations;
--  used for Self

with Unchecked_Conversion;

package body Ada.Task_Termination is

   use type Ada.Task_Identification.Task_Id;

   package STPO renames System.Task_Primitives.Operations;

   -----------------------
   -- Local subprograms --
   -----------------------

   function To_TT is new Unchecked_Conversion
     (System.Tasking.Termination_Handler, Termination_Handler);

   function To_ST is new Unchecked_Conversion
     (Termination_Handler, System.Tasking.Termination_Handler);

   function To_Task_Id is new Unchecked_Conversion
     (Ada.Task_Identification.Task_Id, System.Tasking.Task_Id);

   -----------------------------------
   -- Current_Task_Fallback_Handler --
   -----------------------------------

   function Current_Task_Fallback_Handler return Termination_Handler is
   begin
      return To_TT (System.Tasking.Self.Common.Fall_Back_Handler);
   end Current_Task_Fallback_Handler;

   -------------------------------------
   -- Set_Dependents_Fallback_Handler --
   -------------------------------------

   procedure Set_Dependents_Fallback_Handler
     (Handler : Termination_Handler)
   is
   begin
      STPO.Self.Common.Fall_Back_Handler := To_ST (Handler);
   end Set_Dependents_Fallback_Handler;

   --------------------------
   -- Set_Specific_Handler --
   --------------------------

   procedure Set_Specific_Handler
     (T       : Ada.Task_Identification.Task_Id;
      Handler : Termination_Handler)
   is
   begin
      --  Tasking_Error is raised if the task identified by T has already
      --  terminated. Program_Error is raised if the value of T is
      --  Null_Task_Id.

      if T = Ada.Task_Identification.Null_Task_Id then
         raise Program_Error;
      elsif Ada.Task_Identification.Is_Terminated (T) then
         raise Tasking_Error;
      else
         To_Task_Id (T).Common.Specific_Handler := To_ST (Handler);
      end if;
   end Set_Specific_Handler;

   ----------------------
   -- Specific_Handler --
   ----------------------

   function Specific_Handler
     (T : Ada.Task_Identification.Task_Id) return Termination_Handler
   is
   begin
      --  Tasking_Error is raised if the task identified by T has already
      --  terminated. Program_Error is raised if the value of T is
      --  Null_Task_Id.

      if T = Ada.Task_Identification.Null_Task_Id then
         raise Program_Error;
      elsif Ada.Task_Identification.Is_Terminated (T) then
         raise Tasking_Error;
      else
         return To_TT (To_Task_Id (T).Common.Specific_Handler);
      end if;
   end Specific_Handler;

end Ada.Task_Termination;
