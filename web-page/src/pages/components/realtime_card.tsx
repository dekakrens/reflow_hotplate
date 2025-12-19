import { useWebSocket } from "../../hooks/useWebSocket";

const RealtimeCard = () => {
  const { data } = useWebSocket("ws://192.168.4.1/ws");

  return (
    <div className="bg-base-200 mb-2 h-fit w-full gap-y-2 rounded-lg p-3 text-sm">
      <div className="flex items-center gap-2">
        <div className="flex w-32 justify-between">
          <p className="col-span-1">Temperature</p>
          <p className="col-span-1">:</p>
        </div>

        <p className="flex-auto">{`${data?.temp ?? ""} °C`}</p>
      </div>

      <div className="flex items-center gap-2">
        <div className="flex w-32 justify-between">
          <p className="col-span-1">Setpoint</p>
          <p className="col-span-1">:</p>
        </div>

        <p className="flex-auto">{`${data?.set ?? ""} °C`}</p>
      </div>

      <div className="flex items-center gap-2">
        <div className="flex w-32 justify-between">
          <p className="col-span-1">State</p>
          <p className="col-span-1">:</p>
        </div>

        <p className="flex-auto">{data?.state}</p>
      </div>
    </div>
  );
};

export default RealtimeCard;
