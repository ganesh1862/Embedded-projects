import React, { useState, useEffect } from 'react';
import { CheckCircle, Circle, Target, TrendingUp, Calendar, Code, BookOpen, Users, Award, Clock } from 'lucide-react';

const JobTracker = () => {
  const [currentWeek, setCurrentWeek] = useState(1);
  const [completedTasks, setCompletedTasks] = useState({});
  const [totalScore, setTotalScore] = useState(0);

  const skillAreas = {
    technical: {
      name: "Technical Skills",
      weight: 40,
      icon: Code,
      color: "text-blue-600",
      bgColor: "bg-blue-50",
      skills: [
        { name: "Advanced C/C++", current: 70, target: 90, importance: "Critical" },
        { name: "RTOS (FreeRTOS)", current: 60, target: 85, importance: "Critical" },
        { name: "Communication Protocols", current: 75, target: 90, importance: "Critical" },
        { name: "Hardware Debugging", current: 50, target: 80, importance: "High" },
        { name: "System Design", current: 55, target: 80, importance: "High" },
        { name: "Driver Development", current: 45, target: 75, importance: "High" }
      ]
    },
    platforms: {
      name: "Platform Expertise",
      weight: 25,
      icon: Target,
      color: "text-green-600",
      bgColor: "bg-green-50",
      skills: [
        { name: "ARM Cortex-M", current: 65, target: 85, importance: "Critical" },
        { name: "STM32 Ecosystem", current: 60, target: 80, importance: "High" },
        { name: "ESP32/IoT", current: 70, target: 85, importance: "High" },
        { name: "Linux Embedded", current: 30, target: 65, importance: "Medium" }
      ]
    },
    tools: {
      name: "Professional Tools",
      weight: 20,
      icon: Award,
      color: "text-purple-600",
      bgColor: "bg-purple-50",
      skills: [
        { name: "Git/Version Control", current: 70, target: 85, importance: "Critical" },
        { name: "JTAG/SWD Debugging", current: 55, target: 80, importance: "High" },
        { name: "Oscilloscope/Logic Analyzer", current: 60, target: 80, importance: "High" },
        { name: "Unit Testing", current: 40, target: 70, importance: "Medium" }
      ]
    },
    soft: {
      name: "Professional Skills",
      weight: 15,
      icon: Users,
      color: "text-orange-600",
      bgColor: "bg-orange-50",
      skills: [
        { name: "Technical Communication", current: 60, target: 80, importance: "High" },
        { name: "Problem Solving", current: 70, target: 85, importance: "Critical" },
        { name: "Project Documentation", current: 50, target: 75, importance: "Medium" },
        { name: "Team Collaboration", current: 75, target: 85, importance: "High" }
      ]
    }
  };

  const weeklyPlan = {
    1: {
      focus: "Advanced C/C++ & Code Quality",
      dailyTasks: [
        { day: "Mon", task: "Advanced C concepts: Function pointers, callbacks", duration: "2h", type: "study" },
        { day: "Tue", task: "Memory management deep dive + code practice", duration: "2h", type: "code" },
        { day: "Wed", task: "Implement custom data structures (linked lists, queues)", duration: "2.5h", type: "project" },
        { day: "Thu", task: "Code optimization techniques + profiling", duration: "2h", type: "study" },
        { day: "Fri", task: "Design patterns for embedded systems", duration: "2h", type: "study" },
        { day: "Sat", task: "Mini project: State machine implementation", duration: "3h", type: "project" },
        { day: "Sun", task: "Code review & refactoring practice", duration: "2h", type: "review" }
      ],
      goal: "Master advanced C concepts critical for senior roles",
      targetCompanies: ["Bosch", "Continental", "Tata Elxsi"]
    },
    2: {
      focus: "RTOS Mastery & Multi-threading",
      dailyTasks: [
        { day: "Mon", task: "FreeRTOS advanced features: Queues, semaphores", duration: "2h", type: "study" },
        { day: "Tue", task: "Inter-task communication patterns", duration: "2h", type: "code" },
        { day: "Wed", task: "Priority inversion and solutions", duration: "2h", type: "study" },
        { day: "Thu", task: "Memory management in RTOS", duration: "2h", type: "code" },
        { day: "Fri", task: "RTOS debugging and profiling", duration: "2h", type: "study" },
        { day: "Sat", task: "Project: Multi-task sensor system", duration: "3h", type: "project" },
        { day: "Sun", task: "Performance analysis and optimization", duration: "2h", type: "review" }
      ],
      goal: "Become RTOS expert - key differentiator for 15L+ roles",
      targetCompanies: ["Robert Bosch", "Harman", "Samsung"]
    }
  };

  const interviewPrep = {
    technical: [
      "Explain interrupt handling and ISR best practices",
      "Design a producer-consumer system using RTOS",
      "Debug a deadlock scenario in embedded system",
      "Optimize code for memory and performance",
      "Explain different communication protocols trade-offs"
    ],
    behavioral: [
      "Describe a challenging technical problem you solved",
      "How do you handle tight deadlines in projects?",
      "Explain a time you had to learn new technology quickly",
      "Describe your approach to code reviews and quality",
      "How do you stay updated with embedded technologies?"
    ],
    system: [
      "Design an IoT temperature monitoring system",
      "Architecture for automotive infotainment system",
      "Power management strategy for battery-operated device",
      "Scalable firmware architecture for product family",
      "Safety-critical system design principles"
    ]
  };

  const salaryBenchmarks = {
    "1-2 years": {
      average: "8-12 LPA",
      target: "12-15 LPA",
      top: "15-18 LPA",
      companies: ["Bosch", "Continental", "Tata Elxsi", "Harman", "Samsung"]
    },
    "2-3 years": {
      average: "12-16 LPA",
      target: "15-20 LPA",
      top: "18-25 LPA",
      companies: ["Qualcomm", "Intel", "NVIDIA", "Apple", "Google"]
    }
  };

  const toggleTask = (week, day, taskIndex) => {
    const key = `${week}-${day}-${taskIndex}`;
    setCompletedTasks(prev => ({
      ...prev,
      [key]: !prev[key]
    }));
  };

  const calculateWeeklyProgress = (week) => {
    const weekTasks = weeklyPlan[week]?.dailyTasks || [];
    const completed = weekTasks.filter((_, index) => 
      completedTasks[`${week}-${weekTasks[index].day}-${index}`]
    ).length;
    return Math.round((completed / weekTasks.length) * 100);
  };

  const getTaskTypeColor = (type) => {
    const colors = {
      study: "bg-blue-100 text-blue-800",
      code: "bg-green-100 text-green-800",
      project: "bg-purple-100 text-purple-800",
      review: "bg-orange-100 text-orange-800"
    };
    return colors[type] || "bg-gray-100 text-gray-800";
  };

  return (
    <div className="max-w-7xl mx-auto p-6 bg-gray-50 min-h-screen">
      {/* Header */}
      <div className="bg-white rounded-lg shadow-sm p-6 mb-6">
        <div className="flex justify-between items-center">
          <div>
            <h1 className="text-3xl font-bold text-gray-800">15 LPA Job Target Tracker</h1>
            <p className="text-gray-600 mt-1">1.6 Years Experience → Senior Embedded Engineer</p>
          </div>
          <div className="text-right">
            <div className="text-2xl font-bold text-green-600">Week {currentWeek}</div>
            <div className="text-sm text-gray-500">Progress: {calculateWeeklyProgress(currentWeek)}%</div>
          </div>
        </div>
      </div>

      <div className="grid lg:grid-cols-3 gap-6">
        {/* Current Week Focus */}
        <div className="lg:col-span-2 space-y-6">
          <div className="bg-white rounded-lg shadow-sm p-6">
            <h2 className="text-xl font-semibold mb-4 flex items-center">
              <Calendar className="h-5 w-5 mr-2 text-blue-600" />
              Week {currentWeek}: {weeklyPlan[currentWeek]?.focus}
            </h2>
            
            <div className="mb-4 p-4 bg-blue-50 rounded-lg">
              <h3 className="font-medium text-blue-800 mb-2">This Week's Goal</h3>
              <p className="text-blue-700 text-sm">{weeklyPlan[currentWeek]?.goal}</p>
              <div className="mt-2">
                <span className="text-xs text-blue-600">Target Companies: </span>
                <span className="text-xs text-blue-800">{weeklyPlan[currentWeek]?.targetCompanies.join(", ")}</span>
              </div>
            </div>

            <div className="space-y-3">
              {weeklyPlan[currentWeek]?.dailyTasks.map((task, index) => (
                <div key={index} className="flex items-center p-3 border rounded-lg hover:bg-gray-50">
                  <button
                    onClick={() => toggleTask(currentWeek, task.day, index)}
                    className="mr-3"
                  >
                    {completedTasks[`${currentWeek}-${task.day}-${index}`] ? 
                      <CheckCircle className="h-5 w-5 text-green-600" /> : 
                      <Circle className="h-5 w-5 text-gray-400" />
                    }
                  </button>
                  <div className="flex-1">
                    <div className="flex items-center gap-2 mb-1">
                      <span className="font-medium text-sm">{task.day}</span>
                      <span className={`px-2 py-1 text-xs rounded-full ${getTaskTypeColor(task.type)}`}>
                        {task.type}
                      </span>
                      <span className="text-xs text-gray-500 flex items-center">
                        <Clock className="h-3 w-3 mr-1" />
                        {task.duration}
                      </span>
                    </div>
                    <p className="text-sm text-gray-700">{task.task}</p>
                  </div>
                </div>
              ))}
            </div>

            <div className="mt-4 flex gap-2">
              <button
                onClick={() => setCurrentWeek(Math.max(1, currentWeek - 1))}
                className="px-3 py-1 text-sm bg-gray-200 rounded hover:bg-gray-300"
                disabled={currentWeek === 1}
              >
                Previous Week
              </button>
              <button
                onClick={() => setCurrentWeek(Math.min(12, currentWeek + 1))}
                className="px-3 py-1 text-sm bg-blue-600 text-white rounded hover:bg-blue-700"
                disabled={currentWeek === 12}
              >
                Next Week
              </button>
            </div>
          </div>

          {/* Interview Preparation */}
          <div className="bg-white rounded-lg shadow-sm p-6">
            <h2 className="text-xl font-semibold mb-4 flex items-center">
              <Users className="h-5 w-5 mr-2 text-green-600" />
              Daily Interview Prep (15 mins/day)
            </h2>
            
            <div className="grid md:grid-cols-3 gap-4">
              <div>
                <h3 className="font-medium text-gray-800 mb-2">Technical Questions</h3>
                <div className="space-y-2">
                  {interviewPrep.technical.slice(0, 2).map((q, i) => (
                    <div key={i} className="text-sm text-gray-600 p-2 bg-blue-50 rounded">
                      {q}
                    </div>
                  ))}
                </div>
              </div>
              
              <div>
                <h3 className="font-medium text-gray-800 mb-2">Behavioral</h3>
                <div className="space-y-2">
                  {interviewPrep.behavioral.slice(0, 2).map((q, i) => (
                    <div key={i} className="text-sm text-gray-600 p-2 bg-green-50 rounded">
                      {q}
                    </div>
                  ))}
                </div>
              </div>
              
              <div>
                <h3 className="font-medium text-gray-800 mb-2">System Design</h3>
                <div className="space-y-2">
                  {interviewPrep.system.slice(0, 2).map((q, i) => (
                    <div key={i} className="text-sm text-gray-600 p-2 bg-purple-50 rounded">
                      {q}
                    </div>
                  ))}
                </div>
              </div>
            </div>
          </div>
        </div>

        {/* Skills Progress & Targets */}
        <div className="space-y-6">
          {/* Salary Benchmark */}
          <div className="bg-white rounded-lg shadow-sm p-6">
            <h2 className="text-xl font-semibold mb-4 flex items-center">
              <TrendingUp className="h-5 w-5 mr-2 text-green-600" />
              Salary Target: 15 LPA
            </h2>
            
            <div className="space-y-3">
              <div className="p-3 bg-green-50 rounded-lg">
                <div className="font-medium text-green-800">Your Experience Level</div>
                <div className="text-sm text-green-700">1.6 Years: 8-12 LPA average</div>
                <div className="text-sm font-medium text-green-800">Target: 15 LPA (Top 10%)</div>
              </div>
              
              <div className="text-sm text-gray-600">
                <div className="font-medium mb-2">Top Companies Hiring:</div>
                <ul className="space-y-1">
                  {salaryBenchmarks["1-2 years"].companies.map(company => (
                    <li key={company} className="flex items-center">
                      <span className="w-2 h-2 bg-blue-400 rounded-full mr-2"></span>
                      {company}
                    </li>
                  ))}
                </ul>
              </div>
            </div>
          </div>

          {/* Skills Progress */}
          {Object.entries(skillAreas).map(([key, area]) => (
            <div key={key} className="bg-white rounded-lg shadow-sm p-6">
              <h3 className="font-semibold text-gray-800 mb-4 flex items-center">
                <area.icon className={`h-5 w-5 mr-2 ${area.color}`} />
                {area.name}
                <span className="ml-auto text-sm text-gray-500">Weight: {area.weight}%</span>
              </h3>
              
              <div className="space-y-3">
                {area.skills.map((skill, index) => {
                  const progress = (skill.current / skill.target) * 100;
                  const gap = skill.target - skill.current;
                  return (
                    <div key={index} className="space-y-1">
                      <div className="flex justify-between text-sm">
                        <span className="font-medium">{skill.name}</span>
                        <span className="text-gray-500">{skill.current}%→{skill.target}%</span>
                      </div>
                      <div className="w-full bg-gray-200 rounded-full h-2">
                        <div 
                          className={`h-2 rounded-full ${area.color.includes('blue') ? 'bg-blue-600' : 
                                      area.color.includes('green') ? 'bg-green-600' :
                                      area.color.includes('purple') ? 'bg-purple-600' : 'bg-orange-600'}`}
                          style={{ width: `${progress}%` }}
                        ></div>
                      </div>
                      <div className="flex justify-between text-xs">
                        <span className={skill.importance === 'Critical' ? 'text-red-600 font-medium' : 
                                       skill.importance === 'High' ? 'text-orange-600' : 'text-gray-500'}>
                          {skill.importance}
                        </span>
                        <span className="text-gray-500">Gap: {gap}%</span>
                      </div>
                    </div>
                  );
                })}
              </div>
            </div>
          ))}

          {/* Quick Actions */}
          <div className="bg-white rounded-lg shadow-sm p-6">
            <h3 className="font-semibold text-gray-800 mb-4">Quick Actions</h3>
            <div className="space-y-2">
              <button className="w-full text-left p-2 text-sm bg-blue-50 hover:bg-blue-100 rounded">
                📝 Update Resume with Recent Projects
              </button>
              <button className="w-full text-left p-2 text-sm bg-green-50 hover:bg-green-100 rounded">
                🔍 Apply to 3 Jobs Today
              </button>
              <button className="w-full text-left p-2 text-sm bg-purple-50 hover:bg-purple-100 rounded">
                💼 LinkedIn Profile Optimization
              </button>
              <button className="w-full text-left p-2 text-sm bg-orange-50 hover:bg-orange-100 rounded">
                🎯 Schedule Mock Interview
              </button>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default JobTracker;