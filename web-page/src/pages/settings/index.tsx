import { BiChevronLeft } from "react-icons/bi";
import { useNavigate } from "react-router-dom";
import PIDSettings from "./components/pid_settings";
import { useQuery } from "@tanstack/react-query";
import type { SettingResponse } from "./schema";
import axios from "axios";

const Settings = () => {
  const nav = useNavigate();

  const { data } = useQuery<SettingResponse>({
    queryKey: ["SETTINGS"],
    queryFn: async () => await axios.get("/settings"),
  });

  return (
    <div className="flex flex-col gap-3">
      <div>
        <button type="button" className="btn btn-soft" onClick={() => nav(-1)}>
          <BiChevronLeft className="text-xl" />
          Back
        </button>
      </div>

      <PIDSettings pid={data?.pid} />

      <div className="card card-border bg-base-300 p-3">
        <p>Profile Settings</p>
      </div>
    </div>
  );
};

export default Settings;
