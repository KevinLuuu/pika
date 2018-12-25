// Copyright (c) 2015-present, Qihoo, Inc.  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.

#ifndef PIKA_ADMIN_H_
#define PIKA_ADMIN_H_

#include <sstream>
#include <sys/time.h>
#include <sys/resource.h>
#include <iomanip>

#include "blackwidow/blackwidow.h"
#include "slash/include/slash_string.h"

#include "include/pika_command.h"
#include "include/pika_client_conn.h"

/*
 * Admin
 */
class SlaveofCmd : public Cmd {
 public:
  SlaveofCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag), is_noone_(false), have_offset_(false), filenum_(0), pro_offset_(0) {}
  virtual void Do();

 private:
  std::string master_ip_;
  int64_t master_port_;
  bool is_noone_;
  bool have_offset_;
  int64_t filenum_;
  int64_t pro_offset_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
  virtual void Clear() {
    is_noone_ = false;
    have_offset_ = false;
  }
};

class TrysyncCmd : public Cmd {
 public:
  TrysyncCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  std::string slave_ip_;
  int64_t slave_port_;
  int64_t filenum_;
  int64_t pro_offset_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class AuthCmd : public Cmd {
 public:
  AuthCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  std::string pwd_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class BgsaveCmd : public Cmd {
 public:
  BgsaveCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class BgsaveoffCmd : public Cmd {
 public:
  BgsaveoffCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class CompactCmd : public Cmd {
 public:
  CompactCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  std::string struct_type_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
  virtual void Clear() {
    struct_type_.clear();
  }
};

class PurgelogstoCmd : public Cmd {
 public:
  PurgelogstoCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag), num_(0) {}
  virtual void Do();

 private:
  uint32_t num_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class PingCmd : public Cmd {
 public:
  PingCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class SelectCmd : public Cmd {
 public:
  SelectCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class FlushallCmd : public Cmd {
 public:
  FlushallCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class FlushdbCmd : public Cmd {
 public:
  FlushdbCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  std::string db_name_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
  virtual void Clear() {
    db_name_.clear();
  }
};

class ClientCmd : public Cmd {
 public:
  ClientCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();
  const static std::string CLIENT_LIST_S;
  const static std::string CLIENT_KILL_S;

 private:
  std::string operation_, ip_port_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class InfoCmd : public Cmd {
 public:
  enum InfoSection {
    kInfoErr = 0x0,
    kInfoServer,
    kInfoClients,
    kInfoStats,
    kInfoExecCount,
    kInfoCPU,
    kInfoReplication,
    kInfoKeyspace,
    kInfoBgstats,
    kInfoLog,
    kInfoData,
    kInfo,
    kInfoAll,
    kInfoDoubleMaster
  };

  InfoCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag), rescan_(false), off_(false) {}
  virtual void Do();

 private:
  InfoSection info_section_;
  bool rescan_; //whether to rescan the keyspace
  bool off_;

  const static std::string kInfoSection;
  const static std::string kAllSection;
  const static std::string kServerSection;
  const static std::string kClientsSection;
  const static std::string kStatsSection;
  const static std::string kExecCountSection;
  const static std::string kCPUSection;
  const static std::string kReplicationSection;
  const static std::string kKeyspaceSection;
  const static std::string kLogSection;
  const static std::string kDataSection;
  const static std::string kDoubleMaster;

  virtual void DoInitial(const PikaCmdArgsType& argv) override;
  virtual void Clear() {
    rescan_ = false;
    off_ = false;
  }

  void InfoServer(std::string &info);
  void InfoClients(std::string &info);
  void InfoStats(std::string &info);
  void InfoExecCount(std::string &info);
  void InfoCPU(std::string &info);
  void InfoReplication(std::string &info);
  void InfoKeyspace(std::string &info);
  void InfoLog(std::string &info);
  void InfoData(std::string &info);
  void InfoDoubleMaster(std::string &info);
};

class ShutdownCmd : public Cmd {
 public:
  ShutdownCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class ConfigCmd : public Cmd {
 public:
  ConfigCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  std::vector<std::string> config_args_v_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
  void ConfigGet(std::string &ret);
  void ConfigSet(std::string &ret);
  void ConfigRewrite(std::string &ret);
  void ConfigResetstat(std::string &ret);
};

class MonitorCmd : public Cmd {
 public:
  MonitorCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class DbsizeCmd : public Cmd {
 public:
  DbsizeCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class TimeCmd : public Cmd {
 public:
  TimeCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class DelbackupCmd : public Cmd {
 public:
  DelbackupCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class EchoCmd : public Cmd {
 public:
  EchoCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  std::string body_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};

class ScandbCmd : public Cmd {
 public:
  ScandbCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag), type_(blackwidow::kAll) {}
  virtual void Do();

 private:
  blackwidow::DataType type_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
  virtual void Clear() {
    type_ = blackwidow::kAll;
  }
};

class SlowlogCmd : public Cmd {
 public:
  enum SlowlogCondition{kGET, kLEN, kRESET};
  SlowlogCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag), condition_(kGET) {}
  virtual void Do();
 private:
  int64_t number_;
  SlowlogCmd::SlowlogCondition condition_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
  virtual void Clear() {
    number_ = 10;
    condition_ = kGET;
  }
};

#ifdef TCMALLOC_EXTENSION
class TcmallocCmd : public Cmd {
 public:
  TcmallocCmd(const std::string& name, int arity, uint16_t flag)
      : Cmd(name, arity, flag) {}
  virtual void Do();

 private:
  int64_t type_;
  int64_t rate_;
  virtual void DoInitial(const PikaCmdArgsType& argv) override;
};
#endif
#endif  // PIKA_ADMIN_H_
