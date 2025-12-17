import { useState } from "react";
import Heater from "./heater";
import Reflow from "./reflow";

const Main = () => {
  const [tabActive, setTabActive] = useState<"reflow" | "heater">("reflow");

  return (
    <div>
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

      {tabActive === "reflow" && <Reflow />}
      {tabActive === "heater" && <Heater />}
    </div>
  );
};

export default Main;
