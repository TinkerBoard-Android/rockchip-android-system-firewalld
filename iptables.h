// Copyright 2014 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FIREWALLD_IPTABLES_H_
#define FIREWALLD_IPTABLES_H_

#include <stdint.h>

#include <string>
#include <unordered_set>

#include <base/macros.h>
#include <chromeos/errors/error.h>

#include "firewalld/dbus_adaptor/org.chromium.Firewalld.h"

namespace firewalld {

enum ProtocolEnum { kProtocolTcp, kProtocolUdp };

class IpTables : public org::chromium::FirewalldInterface {
 public:
  IpTables();
  ~IpTables();

  // D-Bus methods.
  bool PunchTcpHole(uint16_t in_port) override;
  bool PunchUdpHole(uint16_t in_port) override;
  bool PlugTcpHole(uint16_t in_port) override;
  bool PlugUdpHole(uint16_t in_port) override;

 protected:
  // Test-only.
  explicit IpTables(const std::string& path);

 private:
  friend class IpTablesTest;

  bool PunchHole(uint16_t port,
                 std::unordered_set<uint16_t>* holes,
                 enum ProtocolEnum protocol);
  bool PlugHole(uint16_t port,
                std::unordered_set<uint16_t>* holes,
                enum ProtocolEnum protocol);

  void PlugAllHoles();

  bool AddAllowRule(enum ProtocolEnum protocol,
                    uint16_t port);
  bool DeleteAllowRule(enum ProtocolEnum protocol,
                       uint16_t port);

  std::string executable_path_;

  // Keep track of firewall holes to avoid adding redundant firewall rules.
  std::unordered_set<uint16_t> tcp_holes_;
  std::unordered_set<uint16_t> udp_holes_;

  DISALLOW_COPY_AND_ASSIGN(IpTables);
};

}  // namespace firewalld

#endif  // FIREWALLD_IPTABLES_H_