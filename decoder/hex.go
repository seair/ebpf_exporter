package decoder

import (
	"fmt"

	"github.com/cloudflare/ebpf_exporter/config"
	"github.com/cloudflare/ebpf_exporter/util"
)

type Hex struct{}

// Decode u64 to hex
func (k *Hex) Decode(in []byte, conf config.Decoder) ([]byte, error) {
	addr := fmt.Sprintf("%x", util.GetHostByteOrder().Uint64(in))

	return []byte(addr), nil
}
