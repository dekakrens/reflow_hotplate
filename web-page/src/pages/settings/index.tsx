import { useQuery } from "@tanstack/react-query";
import axios from "axios";
import { BiChevronLeft } from "react-icons/bi";
import { useNavigate } from "react-router-dom";
import PIDSettings from "./components/pid_settings";
import type { SettingResponse } from "./schema";
import ProfileSettings from "./components/profile_settings";

const Settings = () => {
  const nav = useNavigate();

  const { data } = useQuery<SettingResponse>({
    queryKey: ["SETTINGS"],
    queryFn: async () => await axios.get("/api/settings"),
  });

  return (
    <div className="flex flex-col gap-3">
      <div>
        <button type="button" className="btn btn-soft" onClick={() => nav(-1)}>
          <BiChevronLeft className="text-xl" />
          Back
        </button>
      </div>

      <PIDSettings pid={data?.data.pid} />
      <ProfileSettings profiles={data?.data.profiles} />
    </div>
  );
};

export default Settings;
