import { useState } from "react";
import { IoMdSettings } from "react-icons/io";
import { useNavigate } from "react-router-dom";
import RealtimeCard from "./components/realtime_card";
import Heater from "./heater";
import Reflow from "./reflow";

const Main = () => {
  const nav = useNavigate();

  const [tabActive, setTabActive] = useState<"reflow" | "heater">("reflow");

  return (
    <div className="relative h-full">
      <div className="tabs tabs-box mb-5 grid grid-cols-2 p-2">
        <input
          type="radio"
          name="my_tabs_1"
          className="tab col-span-1"
          aria-label="Reflow"
          onChange={() => setTabActive("reflow")}
          checked={tabActive === "reflow"}
        />
        <input
          type="radio"
          name="my_tabs_1"
          className="tab col-span-1"
          aria-label="Heater"
          onChange={() => setTabActive("heater")}
          checked={tabActive === "heater"}
        />
      </div>

      <div
        role="button"
        className="btn btn-ghost btn-square absolute right-0 bottom-0 text-2xl"
        onClick={() => nav("settings")}
      >
        <IoMdSettings />
      </div>

      <RealtimeCard />

      {tabActive === "reflow" && <Reflow />}
      {tabActive === "heater" && <Heater />}
    </div>
  );
};

export default Main;
