#pragma once

class NetVars {
	private:
		const uintptr_t FindOffset(RecvTable* recv_table, const char* property_name) {
			for (int i = 0; i < recv_table->m_nProps; i++) {
				RecvProp& recv_prop = recv_table->m_pProps[i];

				if (strcmp(recv_prop.m_pVarName, property_name) == 0) {
					return recv_prop.m_Offset;
				}

				if (recv_prop.m_RecvType != SendPropType::DPT_DataTable) {
					continue;
				}

				if (uintptr_t offset = this->FindOffset(recv_prop.m_pDataTable, property_name)) {
					return recv_prop.m_Offset + offset;
				}
			}

			return 0;
		}
	public:
		const uintptr_t GetOffset(const char* class_name, const char* property_name) {
			for (ClientClass* class_ptr = clientdll->GetAllClasses(); class_ptr; class_ptr = class_ptr->m_pNext) {
				if (strcmp(class_ptr->m_pNetworkName, class_name) == 0) {
					return this->FindOffset(class_ptr->m_pRecvTable, property_name);
				}
			}

			return 0;
		}
};