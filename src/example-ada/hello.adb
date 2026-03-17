-- SPDX-License-Identifier: BSD-2-Clause */

-- Copyright (C) 2023 embedded brains GmbH & Co. KG
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions
-- are met:
-- 1. Redistributions of source code must retain the above copyright
--    notice, this list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
-- AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
-- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
-- ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
-- LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
-- CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
-- SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
-- INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
-- CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
-- ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
-- POSSIBILITY OF SUCH DAMAGE.

with Ada.Text_IO; use Ada.Text_IO;

procedure Hello is
  task type Hello_Worker is
    entry Start(hello: String; world: String);
  end Hello_Worker;

  task type World_Worker is
    entry Start(world: String);
  end World_Worker;

  task body Hello_Worker is
    the_world_worker : World_Worker;
  begin
    accept Start(hello: String; world: String)
    do
      Put(hello);
      the_world_worker.Start(world);
    end Start;
  end Hello_Worker;

  task body World_Worker is
  begin
    accept Start(world: String)
    do
      Put_Line(world);
    end Start;
  end World_Worker;

  the_hello_worker : Hello_Worker;
begin
  the_hello_worker.Start("Hello,", " Ada world!");
end Hello;
